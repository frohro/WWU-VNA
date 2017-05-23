/*
 * quadrature.cpp, for setting the local oscillator frequency on
 *                 the 2017 vna project.
 *                 Rob Frohne, adapted from work by Jason Milldrum.
 *
 * Copyright (C) 2015 - 2016 Jason Milldrum <milldrum@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Setting the phase of a clock requires that you manually set the PLL and
 * take the PLL frequency into account when calculation the value to place
 * in the phase register. As shown on page 10 of Silicon Labs Application
 * Note 619 (AN619), the phase register is a 7-bit register, where a bit
 * represents a phase difference of 1/4 the PLL period. Therefore, the best
 * way to get an accurate phase setting is to make the PLL an even multiple
 * of the clock frequency, depending on what phase you need.
 *
 * If you need a 90 degree phase shift (as in many RF applications), then
 * it is quite easy to determine your parameters. Pick a PLL frequency that
 * is an even multiple of your clock frequency (remember that the PLL needs
 * to be in the range of 600 to 900 MHz). Then to set a 90 degree phase shift,
 * you simply enter that multiple into the phase register. Remember when
 * setting multiple outputs to be phase-related to each other, they each need
 * to be referenced to the same PLL.
 * Also note that the minimum frequency you can get quadrature phase difference
 * is 600 MHz/128 = 4.6875 MHz, and the next higher frequency is 601/128 = 4.6963 MHz.
 *
 */

#include "si5351.h"
#include "Wire.h"

extern Si5351 si5351;

void quadrature(unsigned long long freq, unsigned long long pll_freq)
{
  
  /*// We will output 14.1 MHz on CLK2 and CLK1.
  // A PLLA frequency of 705 MHz was chosen to give an even
  // divisor by 14.1 MHz.
  freq = 1410000000ULL;
  pll_freq = 70500000000ULL;
*/
  // Set CLK2 and CLK1 to use PLLA as the MS source.
  // This is not explicitly necessary in v2 of this library,
  // as these are already the default assignments.
   si5351.set_ms_source(SI5351_CLK2, SI5351_PLLA);
   si5351.set_ms_source(SI5351_CLK1, SI5351_PLLA);

  // Set CLK2 and CLK1 to output freq with a fixed PLL frequency
  si5351.set_freq_manual(freq, pll_freq, SI5351_CLK2);
  si5351.set_freq_manual(freq, pll_freq, SI5351_CLK1);

  // Now we can set CLK1 to have a 90 deg phase shift by entering
  // 50 in the CLK1 phase register, since the ratio of the PLL to
  // the clock frequency is 50.
  si5351.set_phase(SI5351_CLK2, 0);
  si5351.set_phase(SI5351_CLK1, 50);

  // We need to reset the PLL before they will be in phase alignment
  si5351.pll_reset(SI5351_PLLA);

 /* // Query a status update and wait a bit to let the Si5351 populate the
  // status flags correctly.
  si5351.update_status();
  delay(500); */
}

