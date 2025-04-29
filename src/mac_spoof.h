#ifndef MAC_SPOOF_H
#define MAC_SPOOF_H

#include <stdbool.h>  // For bool type

void prompt_and_set_mac(void);
void set_rogue_ap_mac(const char *mac);
bool validate_mac_format(const char *mac);

#endif  // MAC_SPOOF_H
