/* original writter Michael Rolig (email: michael_rolig@alumni.macalester.edu)
 *   modified for THANKO USB AM/FM Radio (email: ikegam@mixallow.net)
 */

#define DEBUG false
#define RADIO_VENDID 0x03b4
#define RADIO_DEVID 0x3113

#define READ_EP 0x81
#define WRITE_EP 0x02
#define SEND_PACKET_LENGTH 8

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <hid.h>

void usage(int argc, const char** argv) {
  printf(
    "%s <command> <arg>\n\tchange state of thanko-amfm-radio\n\n"
    "commands:\n"
    " -fm <freqeuncy>    : set FM frequency, e.g. '-fm 77.4'\n"
    " -stop              : stop amfm radio\n", argv[0]);
}

int main(int argc, const char** argv) {
  hid_return ret;
  HIDInterface* hid;
  HIDInterfaceMatcher matcher = { RADIO_VENDID, RADIO_DEVID, NULL, NULL, 0 };

  unsigned char PACKET[SEND_PACKET_LENGTH] = { 0x28, 0xe2, 0x06, 0x03, 0x29, 0x52, 0x00, 0x10 };
  unsigned char ZERO[0];
  unsigned short encodedFreq;
  unsigned int freq;
  unsigned int tmp;

  if ( argc == 3 ) {
    if ( strcmp(argv[1], "-fm") == 0 ) {
      freq = ((int)(atof(argv[2]) * 100)-7600)*2/10+0xc6;
      PACKET[1] = (freq & 0x00ff);
      PACKET[2] = (freq >> 8) + 0x06;
    }
  } else if ( argc == 2 ) {
    if ( strcmp(argv[1], "-stop") == 0) {
      PACKET[5] = 0x72;
    }
  } else {
    usage(argc,argv);
  }


  if (DEBUG) {
    hid_set_debug(HID_DEBUG_ALL);
    hid_set_debug_stream(stderr);
    hid_set_usb_debug(0);
  }

  ret = hid_init();
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_init failed with return code %d\n", ret);
    return 1;
  }

  hid = hid_new_HIDInterface();
  if (hid == 0) {
    fprintf(stderr, "hid_new_HIDInterface() failed, out of memory?\n");
    return 1;
  }

  ret = hid_force_open(hid, 0, &matcher, 3);
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_force_open failed with return code %d\n", ret);
    return 1;
  }

  ret = hid_interrupt_write(hid, WRITE_EP, (char*)ZERO, 0, 10000);
  if (ret != HID_RET_SUCCESS) fprintf(stderr, "hid_interrupt_write failed with return code %d\n", ret);

  ret = hid_interrupt_write(hid, WRITE_EP, (char*)PACKET, SEND_PACKET_LENGTH, 10000);
  if (ret != HID_RET_SUCCESS) fprintf(stderr, "hid_interrupt_write failed with return code %d\n", ret);

  ret = hid_close(hid);
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_close failed with return code %d\n", ret);
    return 1;
  }

  hid_delete_HIDInterface(&hid);

  ret = hid_cleanup();
  if (ret != HID_RET_SUCCESS) {
    fprintf(stderr, "hid_cleanup failed with return code %d\n", ret);
    return 1;
  }

  return 0;
}
