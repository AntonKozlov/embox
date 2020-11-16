/**
 * @file
 *
 * @brief Jiffies implementaion.
 * @details jiffies are generating by mostly precision clock source with event device.
 *
 * @date 10.04.2012
 * @author Anton Bondarev
 */

#include <string.h>
#include <kernel/time/clock_source.h>
#include <kernel/time/time.h>

#define HZ      OPTION_GET(NUMBER, hz)
#define CS_NAME OPTION_STRING_GET(cs_name)

const struct clock_source *cs_jiffies;

clock_t clock_sys_ticks(void) {
	if (!cs_jiffies) {
		return 0;
	}
	return (clock_t)cs_jiffies->jiffies;
}

clock_t ns2jiffies(time64_t ns) {
	assert(cs_jiffies->event_device);
	return ns_to_clock(cs_jiffies->event_device->event_hz, ns);
}

clock_t ms2jiffies(time64_t ms) {
	return ns2jiffies(ms * 1000000);
}

time64_t jiffies2ms(clock_t jiff) {
	return clock_to_ns(cs_jiffies->event_device->event_hz, jiff) / 1000000;
}

uint32_t clock_freq(void) {
	return cs_jiffies->event_device->event_hz;
}

int jiffies_init(void) {
	struct clock_source *cs;

	if (cs_jiffies && strcmp(CS_NAME, "")) {
		/* Jiffies clock source already found and assigned, do nothing. */
		return 0;
	}

	if (!strcmp(CS_NAME, "")) {
		cs = clock_source_get_best(CS_WITH_IRQ);
		assert(cs);
	} else {
		cs = clock_source_get_by_name(CS_NAME);
		assertf(cs, "clock source \"%s\" not found", CS_NAME);
	}

	cs_jiffies = cs;

	clock_source_set_periodic(cs, HZ);

	return 0;
}
