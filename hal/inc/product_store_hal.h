/**
 ******************************************************************************
  Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
*/

#ifndef APPSTORE_HAL_H
#define	APPSTORE_HAL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum ProductStoreIndex
{
    /**
     * The persisted Product ID, 0xFFFF if none set.
     */
    PRODUCT_STORE_ID = 0,

    /**
     * The persisted product version. 0xFFFF if none set.
     */
    PRODUCT_STORE_VERSION = 1


} ProductStoreIndex;

/**
 * Sets the value at a specific product store index.
 * @return The previous value.
 */
uint16_t HAL_SetProductStore(ProductStoreIndex index, uint16_t value);

/**
 * Fetches the value at a given index in the product store.
 * @param index
 * @return
 */
uint16_t HAL_GetProductStore(ProductStoreIndex index);

#ifdef	__cplusplus
}
#endif

#endif	/* APPSTORE_HAL_H */
