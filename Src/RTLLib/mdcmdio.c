/*
* Copyright (C) 2010 Realtek Semiconductor Corp.
* All Rights Reserved.
*
* This program is the proprietary software of Realtek Semiconductor
* Corporation and/or its licensors, and only be used, duplicated,
* modified or distributed under the authorized license from Realtek.
*
* ANY USE OF THE SOFTWARE OTEHR THAN AS AUTHORIZED UNDER 
* THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
* 
* $Revision: 10083 $ 
* $Date: 2010-06-07 11:18:41 +0800 (星期一, 07 六月 2010) $
*
* Purpose : MDC/MDIO example code
*
* Feature :  This file consists of following modules:
*                
*
*/


#include "../RTLLib/mdcmdio.h"      /*RTL8651B file*/

#include "..\Board\ChipSE0165B.h"

/* Function Name:
 *      smiRead
 * Description:
 *      Read data from phy register
 * Input:
 *      phyad   - PHY address (0~31)
 *      regad   -  Register address (0 ~31) 
 * Output:
 *      data    -  Register value 
 * Return:
 *      SUCCESS         -  Success
 *      FAILED            -  Failure
 * Note:
 *     This function could read register through MDC/MDIO serial 
 *     interface, and it is platform  related. It use two GPIO pins 
 *     to simulate MDC/MDIO timing. MDC is sourced by the Station Management 
 *     entity to the PHY as the timing reference for transfer of information
 *     on the MDIO signal. MDC is an aperiodic signal that has no maximum high 
 *     or low times. The minimum high and low times for MDC shall be 160 ns each, 
 *     and the minimum period for MDC shall be 400 ns. Obeying frame format defined
 *     by IEEE802.3 standard, you could access Phy registers. If you want to 
 *     port it to other CPU, please modify static functions which are called 
 *      by this function.
 */
int8 smiRead(uint8 phyad, uint8 regad, uint16 * regv)
{
    if ((phyad > 31) || (regad > 31) || (regv == NULL))  
        return FAILED;
    *regv = readMDIO(phyad, regad);
    return  SUCCESS;
}

/* Function Name:
 *      smiWrite
 * Description:
 *      Write data to Phy register
 * Input:
 *      phyad   - PHY address (0~31)
 *      regad   -  Register address (0 ~31)
 *      data    -  Data to be written into Phy register
 * Output:
 *      none
 * Return:
 *      SUCCESS         -  Success
 *      FAILED            -  Failure
 * Note:
 *     This function could read register through MDC/MDIO serial 
 *     interface, and it is platform  related. It use two GPIO pins 
 *     to simulate MDC/MDIO timing. MDC is sourced by the Station Management 
 *     entity to the PHY as the timing reference for transfer of information
 *     on the MDIO signal. MDC is an aperiodic signal that has no maximum high 
 *     or low times. The minimum high and low times for MDC shall be 160 ns each, 
 *     and the minimum period for MDC shall be 400 ns. Obeying frame format defined
 *     by IEEE802.3 standard, you could access Phy registers. If you want to 
 *     port it to other CPU, please modify static functions which are called 
*      by this function.
 */

int8 smiWrite(uint8 phyad, uint8 regad, uint16 regv)
{
    if ((phyad > 31) || (regad > 31) || (regv > 0xFFFF))  
        return FAILED;
    writeMDIO(phyad, regad, regv);
    return SUCCESS; 
}





/* Function Name:
 *      smiReadBit
 * Description:
 *      Read one bit of PHY register
 * Input:
 *      phyad   - PHY address (0~31)
 *      regad   -  Register address (0 ~31) 
 *      bit       -  Register bit (0~15)   
 * Output:
 *      pdata    - the pointer of  Register bit value 
 * Return:
 *      SUCCESS         -  Success
 *      FAILED            -  Failure
 * Note:
 */

int8 smiReadBit(uint8 phyad, uint8 regad, uint8 pos, uint8 * bv)
{
    uint16 regData;

    if ((phyad > 31) || (regad > 31) || (pos > 15) || (bv == NULL) ) 
        return  FAILED;
    
    if(pos>=16)
        * bv = 0;
    else 
    {
        smiRead(phyad, regad, &regData);
        if(regData & (1<<pos)) 
            * bv = 1;
        else
            * bv = 0;
    }
    return SUCCESS;
}

/* Function Name:
 *      smiWriteBit
 * Description:
 *      Write one bit of PHY register
 * Input:
 *      phyad   - PHY address (0~31)
 *      regad   -  Register address (0 ~31) 
 *      bit       -  Register bit (0~15)   
 *      data     -  Bit value to be written
 * Output:
 *      none
 * Return:
 *      SUCCESS         -  Success
 *      FAILED            -  Failure
 * Note:
 */

int8 smiWriteBit(uint8 phyad, uint8 regad, uint8 pos, uint8 bv)
{
    uint16 regData;
    
    if ((phyad > 31) || (regad > 31) || (pos > 15) || (bv > 1) ) 
        return  FAILED;
    smiRead(phyad, regad, &regData);
    if(bv) 
        regData = regData | (1<<pos);
    else
        regData = regData & ~(1<<pos);
    smiWrite(phyad, regad, regData);
    return SUCCESS;
}




















