#define ENABLE_FAST_OUTPUT

#ifndef HARDWARE_SETUP_H
#define HARDWARE_SETUP_H

#define PORT_VID	PORTD
#define	DDR_VID		DDRD
#define	VID_PIN		7
#define PORT_SYNC	PORTB
#define DDR_SYNC	DDRB
#define SYNC_PIN	1

#define BLD_HWS		"bld	r16,7\n\t"
#define BST_HWS		"bst	r16,7\n\t"
#define ANDI_HWS	"andi	r16,0x7F\n"
#endif
