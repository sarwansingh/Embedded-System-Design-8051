#ifndef KBD_H
#define KBD_H

void KBD_Init(void);
int KBD_kbhit(void);	// returns true if a key is pending
int KBD_rdkbd(void);	// waits until a key is pressed and then returns the keycode

#endif /* KBD_H */
