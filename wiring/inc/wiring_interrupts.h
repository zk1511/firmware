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

#ifndef WIRING_INTERRUPTS_H_
#define WIRING_INTERRUPTS_H_

#include "wiring.h"
#include "interrupts_hal.h"
#include <functional>

typedef std::function<void()> wiring_interrupt_handler_t;
typedef void (*raw_interrupt_handler_t)(void);

/*
 * GPIO Interrupts
 */
bool attachInterrupt(uint16_t pin, wiring_interrupt_handler_t handler, InterruptMode mode, int8_t priority = -1, uint8_t subpriority = 0);
bool attachInterrupt(uint16_t pin, raw_interrupt_handler_t handler, InterruptMode mode, int8_t priority = -1, uint8_t subpriority = 0);
template <typename T>
bool attachInterrupt(uint16_t pin, void (T::*handler)(), T *instance, InterruptMode mode, int8_t priority = -1, uint8_t subpriority = 0) {
    using namespace std::placeholders;
    return attachInterrupt(pin, std::bind(handler, instance), mode, priority, subpriority);
}
void detachInterrupt(uint16_t pin);
void interrupts(void);
void noInterrupts(void);

/*
 * System Interrupts
 */
bool attachSystemInterrupt(hal_irq_t irq, wiring_interrupt_handler_t handler);

/**
 * Removes all registered handlers from the given system interrupt.
 * @param irq   The interrupt from which all handlers are removed.
 * @return {@code true} if handlers were removed.
 */
bool detachSystemInterrupt(hal_irq_t irq);


class AtomicSection {
	int prev;
public:
	AtomicSection() {
		prev = HAL_disable_irq();
	}

	~AtomicSection() {
		HAL_enable_irq(prev);
	}
};

#define ATOMIC_BLOCK() 	for (bool __todo=true; __todo;) for (AtomicSection __as; __todo; __todo=false)



#endif /* __WIRING_INTERRUPTS_H_ */