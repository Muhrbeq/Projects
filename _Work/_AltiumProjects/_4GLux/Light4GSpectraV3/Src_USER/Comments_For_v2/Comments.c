/*
 * Comments.c
 *
 *  Created on: 12 mars 2019
 *      Author: Rasmus.Muhrbeck
 */


/*This document is created to have all updates for v2 on one place

 1. What needs to be changed in the structure to be able to send ONLY the missing packets? This means that if a send of 100 packets is made,
 Packet 4, 26, 56, and 83 fails. In the current code that will result in sending of all 100 packets again. How can this be changed so that
 it only send the missing?

 - No idea so far.

 2. Make the sleep function more dynamic. In the current code the module only sleeps when both the radio and sensors are in IDLE mode.
 This can be made more dynamic as well so that the module can sleep while having, for example, the Radio GPIO pin enabled. This means
 that the radio can search for network (which can take minutes) while bringing the current consumption down to a minimum.

 - Make a more dynamic sleep function that does not turn off everything. (leaves radio on for example)













































 */
