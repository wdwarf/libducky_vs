/*
 * WCRC.h
 *
 *  Created on: Aug 2, 2012
 *      Author: ducky
 */

#ifndef DUCKY_ENCRYPT_CRC_H_
#define DUCKY_ENCRYPT_CRC_H_

#include <libducky/Object.h>

namespace ducky {
namespace encrypt {

class CRC: virtual public Object {
public:
	static unsigned short CRC16(const unsigned char* ptr, unsigned int len);

private:
	static unsigned short CRC16_1(const unsigned char *ptr, unsigned int len);
	static unsigned short CRC16_2(const unsigned char *ptr, unsigned int len);
	static unsigned short CRC16_3(const unsigned char *ptr, unsigned int len);

	CRC();
	virtual ~CRC();
};

} /* namespace encrypt */
} /* namespace ducky */

#endif /* WCRC_H_ */
