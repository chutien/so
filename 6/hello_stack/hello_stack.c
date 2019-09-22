#include <minix/drivers.h>
#include <minix/chardriver.h>
#include <stdio.h>
#include <stdlib.h>
#include <minix/ds.h>
#include <assert.h>
#include "hello_stack.h"

typedef unsigned char byte;

/*
 * Function prototypes for the hello stack driver.
 */
static int hello_stack_open(devminor_t minor, int access, endpoint_t user_endpt);
static int hello_stack_close(devminor_t minor);
static ssize_t hello_stack_read(devminor_t minor, u64_t position, endpoint_t endpt,
                          cp_grant_id_t grant, size_t size, int flags, cdev_id_t id);
static ssize_t hello_stack_write(devminor_t minor, u64_t position,
                             endpoint_t endpt, cp_grant_id_t grant, size_t size, int flags,
                             cdev_id_t id);

/* SEF functions and variables. */
static void sef_local_startup(void);
static int sef_cb_init(int type, sef_init_info_t *info);
static int sef_cb_lu_state_save(int);
static int lu_state_restore(void);

/* Entry points to the hello_stack driver. */
static struct chardriver hello_stack_tab =
  {
    .cdr_open	= hello_stack_open,
    .cdr_close = hello_stack_close,
    .cdr_read	= hello_stack_read,
    .cdr_write = hello_stack_write,
  };

/** State variable to count the number of times the device has been opened.
 * Note that this is not the regular type of open counter: it never decreases.
 */
static int open_counter;

byte *buffer;
size_t buf_cap;
size_t buf_len;

static int hello_stack_open(devminor_t UNUSED(minor), int UNUSED(access),
    endpoint_t UNUSED(user_endpt))
{
    return OK;
}

static int hello_stack_close(devminor_t UNUSED(minor))
{
    return OK;
}

static ssize_t hello_stack_read(devminor_t UNUSED(minor), u64_t UNUSED(position),
                                endpoint_t endpt, cp_grant_id_t grant, size_t size, int UNUSED(flags), cdev_id_t UNUSED(id))
{
  byte *ptr;
  int ret;

  /* Limit the read size if needed. */
  if (buf_len == 0)
    return 0;
  if (size > buf_len)
    size = buf_len;	/* limit size */

  /* Copy the requested part to the caller. */
  ptr = buffer + buf_len - size;
  if ((ret = sys_safecopyto(endpt, grant, 0, (vir_bytes) ptr, size)) != OK)
    return ret;

  /* Adjust stack. */
  buf_len -= size;
  if (4 * buf_len <= buf_cap && buf_cap > 1) {
    buf_cap /= 2;
    buffer = realloc(buffer, sizeof(byte) * buf_cap);
  }
  
  /* Return the number of bytes read. */
  return size;
}

static ssize_t hello_stack_write(devminor_t UNUSED(minor), u64_t UNUSED(position),
                                 endpoint_t endpt, cp_grant_id_t grant, size_t size, int UNUSED(flags), cdev_id_t UNUSED(id))
{
  byte *ptr;
  int ret;
  if (size == 0)
    return 0;

  while (size > buf_cap - buf_len)
    buf_cap *= 2;
  buffer = realloc(buffer, sizeof(byte) * buf_cap);

  ptr = buffer + buf_len;
  if ((ret = sys_safecopyfrom(endpt, grant, 0, (vir_bytes) ptr, size)) != OK)
    return ret;

  buf_len += size;
  return size;
}

static int sef_cb_lu_state_save(int UNUSED(state)) {

  /* Save the state. */
  ds_publish_mem("buffer", buffer, buf_cap, DSF_OVERWRITE);
  ds_publish_u32("buf_len", buf_len, DSF_OVERWRITE);
  ds_publish_u32("buf_cap", buf_cap, DSF_OVERWRITE);
  free(buffer);
  return OK;
}

static int lu_state_restore() {
/* Restore the state. */
    u32_t value;

    ds_retrieve_u32("buf_cap", &value);
    buf_cap = (size_t) value;
    ds_delete_u32("buf_cap");

    ds_retrieve_u32("buf_len", &value);
    buf_len = (size_t) value;
    ds_delete_u32("buf_len");

    buffer = malloc(sizeof(byte) * buf_cap);
    ds_retrieve_mem("buffer", buffer, &buf_cap);
    ds_delete_mem("buffer");

    return OK;
}

static void sef_local_startup()
{
    /*
     * Register init callbacks. Use the same function for all event types
     */
    sef_setcb_init_fresh(sef_cb_init);
    sef_setcb_init_lu(sef_cb_init);
    sef_setcb_init_restart(sef_cb_init);

    /*
     * Register live update callbacks.
     */
    /* - Agree to update immediately when LU is requested in a valid state. */
    sef_setcb_lu_prepare(sef_cb_lu_prepare_always_ready);
    /* - Support live update starting from any standard state. */
    sef_setcb_lu_state_isvalid(sef_cb_lu_state_isvalid_standard);
    /* - Register a custom routine to save the state. */
    sef_setcb_lu_state_save(sef_cb_lu_state_save);

    /* Let SEF perform startup. */
    sef_startup();
}

static int sef_cb_init(int type, sef_init_info_t *UNUSED(info))
{
/* Initialize the hello_stack driver. */
    int do_announce_driver = TRUE;

    open_counter = 0;
    switch(type) {
        case SEF_INIT_FRESH:
          buffer = malloc(sizeof(byte) * DEVICE_SIZE);
          buf_cap = DEVICE_SIZE;
          assert(buffer);
          for (buf_len = 0; buf_len < buf_cap; ++buf_len) {
            buffer[buf_len] = 'a' + buf_len % 3;
          }
        break;

        case SEF_INIT_LU:
          /* Restore the state. */
          lu_state_restore();
          do_announce_driver = FALSE;
        break;

        case SEF_INIT_RESTART:

        break;
    }

    /* Announce we are up when necessary. */
    if (do_announce_driver) {
        chardriver_announce();
    }

    /* Initialization completed successfully. */
    return OK;
}

int main(void)
{
    /*
     * Perform initialization.
     */
    sef_local_startup();

    /*
     * Run the main loop.
     */
    chardriver_task(&hello_stack_tab);
    return OK;
}

