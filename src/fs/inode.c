/**
 * @file
 * @brief pool of nodes
 *
 * @date 06.10.10
 * @author Nikolay Korotky
 * @author Eldar Abusalimov
 */

#include <string.h>
#include <limits.h>
#include <errno.h>

#include <mem/misc/pool.h>

#include <fs/inode.h>
#include <fs/vfs.h>

#include <framework/mod/options.h>

#define MAX_NODE_QUANTITY OPTION_GET(NUMBER,fnode_quantity)

struct node_tuple {
	struct inode node;
	struct nas nas;
	struct node_fi fi;
};

POOL_DEF(node_pool, struct node_tuple, MAX_NODE_QUANTITY);

static inline int flock_init(struct inode *node) {
	/* flock initialization */
	mutex_init(&node->flock.exlock);
	node->flock.shlock_count = 0;
	dlist_init(&node->flock.shlock_holders);
	spin_init(&node->flock.flock_guard, __SPIN_UNLOCKED);

	return ENOERR;
}

struct inode *inode_new(struct super_block *sb) {
	struct node_tuple *nt;
	struct inode *node;
	struct nas *nas;

	nt = pool_alloc(&node_pool);
	if (!nt) {
		return NULL;
	}

	memset(nt, 0, sizeof(struct node_tuple));

	node = &nt->node;
	nas = &nt->nas;

	node->nas = nas;
	nas->node = node;

	nas->fi = &nt->fi;

	tree_link_init(&node->tree_link);

	node->mounted = 0;

	flock_init(node);

	node->i_nlink = 0;
	slist_link_init(&node->dirent_link);

	return node;
}

void inode_del(struct inode *node) {
	node->i_nlink --;
	if (0 == node->i_nlink) {
		vfs_del_leaf(node);
	}
}

struct inode *node_alloc(const char *name, size_t name_len) {
	struct inode *node;

	if (!name_len) {
		name_len = strlen(name);
	}

	if (!*name || name_len > NAME_MAX) {
		/* TODO behave more clever here? -- Eldar */
		return NULL;
	}

	node = inode_new(NULL);
	if (!node) {
		return NULL;
	}

	strncpy((char *) node->name, name, name_len);
	node->name[name_len] = '\0';

	/* it's for permanent linked inode to file tree */
	node->i_nlink ++;

	return node;
}

void node_free(struct inode *node) {
	pool_free(&node_pool, member_cast_out(node, struct node_tuple, node));
}
