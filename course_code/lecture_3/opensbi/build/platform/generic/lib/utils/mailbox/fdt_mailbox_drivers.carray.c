// Generated with carray.sh from /home/hwb/jyyos/os2025/course_code/lecture_3/opensbi/lib/utils/mailbox/fdt_mailbox_drivers.carray
// DO NOT EDIT THIS FILE DIRECTLY

#include <sbi_utils/mailbox/fdt_mailbox.h>

extern const struct fdt_mailbox fdt_mailbox_rpmi_shmem;

const struct fdt_driver *const fdt_mailbox_drivers[] = {
	&fdt_mailbox_rpmi_shmem.driver,
	NULL
};
