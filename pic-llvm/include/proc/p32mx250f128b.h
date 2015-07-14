/*-------------------------------------------------------------------------
 * PIC32MX250F128B processor header
 * Build date : Dec 02 2014
 *
 * Copyright (c) 2014, Microchip Technology Inc. and its subsidiaries ("Microchip")
 * All rights reserved.
 * 
 * This software is developed by Microchip Technology Inc. and its
 * subsidiaries ("Microchip").
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are 
 * met:
 * 
 * 1.      Redistributions of source code must retain the above copyright
 *         notice, this list of conditions and the following disclaimer.
 * 2.      Redistributions in binary form must reproduce the above 
 *         copyright notice, this list of conditions and the following 
 *         disclaimer in the documentation and/or other materials provided 
 *         with the distribution.
 * 3.      Microchip's name may not be used to endorse or promote products
 *         derived from this software without specific prior written 
 *         permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY MICROCHIP "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL MICROCHIP BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING BUT NOT LIMITED TO
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWSOEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

/* NOTE: This is a stripped version of the full header */

#pragma once
#ifndef __32MX250F128B_H
#define __32MX250F128B_H

#if defined (__LANGUAGE_C__) || defined (__LANGUAGE_C_PLUS_PLUS)

#ifdef __cplusplus
extern "C" {
#endif

extern volatile unsigned int        WDTCON __attribute__((section("sfrs")));
extern volatile unsigned int        WDTCONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        WDTCONSET __attribute__((section("sfrs")));
extern volatile unsigned int        WDTCONINV __attribute__((section("sfrs")));
extern volatile unsigned int        RTCCON __attribute__((section("sfrs")));
extern volatile unsigned int        RTCCONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        RTCCONSET __attribute__((section("sfrs")));
extern volatile unsigned int        RTCCONINV __attribute__((section("sfrs")));
extern volatile unsigned int        RTCALRM __attribute__((section("sfrs")));
extern volatile unsigned int        RTCALRMCLR __attribute__((section("sfrs")));
extern volatile unsigned int        RTCALRMSET __attribute__((section("sfrs")));
extern volatile unsigned int        RTCALRMINV __attribute__((section("sfrs")));
extern volatile unsigned int        RTCTIME __attribute__((section("sfrs")));
extern volatile unsigned int        RTCTIMECLR __attribute__((section("sfrs")));
extern volatile unsigned int        RTCTIMESET __attribute__((section("sfrs")));
extern volatile unsigned int        RTCTIMEINV __attribute__((section("sfrs")));
extern volatile unsigned int        RTCDATE __attribute__((section("sfrs")));
extern volatile unsigned int        RTCDATECLR __attribute__((section("sfrs")));
extern volatile unsigned int        RTCDATESET __attribute__((section("sfrs")));
extern volatile unsigned int        RTCDATEINV __attribute__((section("sfrs")));
extern volatile unsigned int        ALRMTIME __attribute__((section("sfrs")));
extern volatile unsigned int        ALRMTIMECLR __attribute__((section("sfrs")));
extern volatile unsigned int        ALRMTIMESET __attribute__((section("sfrs")));
extern volatile unsigned int        ALRMTIMEINV __attribute__((section("sfrs")));
extern volatile unsigned int        ALRMDATE __attribute__((section("sfrs")));
extern volatile unsigned int        ALRMDATECLR __attribute__((section("sfrs")));
extern volatile unsigned int        ALRMDATESET __attribute__((section("sfrs")));
extern volatile unsigned int        ALRMDATEINV __attribute__((section("sfrs")));
extern volatile unsigned int        T1CON __attribute__((section("sfrs")));
extern volatile unsigned int        T1CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        T1CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        T1CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        TMR1 __attribute__((section("sfrs")));
extern volatile unsigned int        TMR1CLR __attribute__((section("sfrs")));
extern volatile unsigned int        TMR1SET __attribute__((section("sfrs")));
extern volatile unsigned int        TMR1INV __attribute__((section("sfrs")));
extern volatile unsigned int        PR1 __attribute__((section("sfrs")));
extern volatile unsigned int        PR1CLR __attribute__((section("sfrs")));
extern volatile unsigned int        PR1SET __attribute__((section("sfrs")));
extern volatile unsigned int        PR1INV __attribute__((section("sfrs")));
extern volatile unsigned int        T2CON __attribute__((section("sfrs")));
extern volatile unsigned int        T2CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        T2CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        T2CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        TMR2 __attribute__((section("sfrs")));
extern volatile unsigned int        TMR2CLR __attribute__((section("sfrs")));
extern volatile unsigned int        TMR2SET __attribute__((section("sfrs")));
extern volatile unsigned int        TMR2INV __attribute__((section("sfrs")));
extern volatile unsigned int        PR2 __attribute__((section("sfrs")));
extern volatile unsigned int        PR2CLR __attribute__((section("sfrs")));
extern volatile unsigned int        PR2SET __attribute__((section("sfrs")));
extern volatile unsigned int        PR2INV __attribute__((section("sfrs")));
extern volatile unsigned int        T3CON __attribute__((section("sfrs")));
extern volatile unsigned int        T3CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        T3CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        T3CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        TMR3 __attribute__((section("sfrs")));
extern volatile unsigned int        TMR3CLR __attribute__((section("sfrs")));
extern volatile unsigned int        TMR3SET __attribute__((section("sfrs")));
extern volatile unsigned int        TMR3INV __attribute__((section("sfrs")));
extern volatile unsigned int        PR3 __attribute__((section("sfrs")));
extern volatile unsigned int        PR3CLR __attribute__((section("sfrs")));
extern volatile unsigned int        PR3SET __attribute__((section("sfrs")));
extern volatile unsigned int        PR3INV __attribute__((section("sfrs")));
extern volatile unsigned int        T4CON __attribute__((section("sfrs")));
extern volatile unsigned int        T4CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        T4CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        T4CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        TMR4 __attribute__((section("sfrs")));
extern volatile unsigned int        TMR4CLR __attribute__((section("sfrs")));
extern volatile unsigned int        TMR4SET __attribute__((section("sfrs")));
extern volatile unsigned int        TMR4INV __attribute__((section("sfrs")));
extern volatile unsigned int        PR4 __attribute__((section("sfrs")));
extern volatile unsigned int        PR4CLR __attribute__((section("sfrs")));
extern volatile unsigned int        PR4SET __attribute__((section("sfrs")));
extern volatile unsigned int        PR4INV __attribute__((section("sfrs")));
extern volatile unsigned int        T5CON __attribute__((section("sfrs")));
extern volatile unsigned int        T5CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        T5CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        T5CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        TMR5 __attribute__((section("sfrs")));
extern volatile unsigned int        TMR5CLR __attribute__((section("sfrs")));
extern volatile unsigned int        TMR5SET __attribute__((section("sfrs")));
extern volatile unsigned int        TMR5INV __attribute__((section("sfrs")));
extern volatile unsigned int        PR5 __attribute__((section("sfrs")));
extern volatile unsigned int        PR5CLR __attribute__((section("sfrs")));
extern volatile unsigned int        PR5SET __attribute__((section("sfrs")));
extern volatile unsigned int        PR5INV __attribute__((section("sfrs")));
extern volatile unsigned int        IC1CON __attribute__((section("sfrs")));
extern volatile unsigned int        IC1CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        IC1CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        IC1CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        IC1BUF __attribute__((section("sfrs")));
extern volatile unsigned int        IC2CON __attribute__((section("sfrs")));
extern volatile unsigned int        IC2CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        IC2CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        IC2CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        IC2BUF __attribute__((section("sfrs")));
extern volatile unsigned int        IC3CON __attribute__((section("sfrs")));
extern volatile unsigned int        IC3CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        IC3CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        IC3CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        IC3BUF __attribute__((section("sfrs")));
extern volatile unsigned int        IC4CON __attribute__((section("sfrs")));
extern volatile unsigned int        IC4CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        IC4CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        IC4CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        IC4BUF __attribute__((section("sfrs")));
extern volatile unsigned int        IC5CON __attribute__((section("sfrs")));
extern volatile unsigned int        IC5CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        IC5CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        IC5CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        IC5BUF __attribute__((section("sfrs")));
extern volatile unsigned int        OC1CON __attribute__((section("sfrs")));
extern volatile unsigned int        OC1CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        OC1CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        OC1CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        OC1R __attribute__((section("sfrs")));
extern volatile unsigned int        OC1RCLR __attribute__((section("sfrs")));
extern volatile unsigned int        OC1RSET __attribute__((section("sfrs")));
extern volatile unsigned int        OC1RINV __attribute__((section("sfrs")));
extern volatile unsigned int        OC1RS __attribute__((section("sfrs")));
extern volatile unsigned int        OC1RSCLR __attribute__((section("sfrs")));
extern volatile unsigned int        OC1RSSET __attribute__((section("sfrs")));
extern volatile unsigned int        OC1RSINV __attribute__((section("sfrs")));
extern volatile unsigned int        OC2CON __attribute__((section("sfrs")));
extern volatile unsigned int        OC2CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        OC2CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        OC2CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        OC2R __attribute__((section("sfrs")));
extern volatile unsigned int        OC2RCLR __attribute__((section("sfrs")));
extern volatile unsigned int        OC2RSET __attribute__((section("sfrs")));
extern volatile unsigned int        OC2RINV __attribute__((section("sfrs")));
extern volatile unsigned int        OC2RS __attribute__((section("sfrs")));
extern volatile unsigned int        OC2RSCLR __attribute__((section("sfrs")));
extern volatile unsigned int        OC2RSSET __attribute__((section("sfrs")));
extern volatile unsigned int        OC2RSINV __attribute__((section("sfrs")));
extern volatile unsigned int        OC3CON __attribute__((section("sfrs")));
extern volatile unsigned int        OC3CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        OC3CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        OC3CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        OC3R __attribute__((section("sfrs")));
extern volatile unsigned int        OC3RCLR __attribute__((section("sfrs")));
extern volatile unsigned int        OC3RSET __attribute__((section("sfrs")));
extern volatile unsigned int        OC3RINV __attribute__((section("sfrs")));
extern volatile unsigned int        OC3RS __attribute__((section("sfrs")));
extern volatile unsigned int        OC3RSCLR __attribute__((section("sfrs")));
extern volatile unsigned int        OC3RSSET __attribute__((section("sfrs")));
extern volatile unsigned int        OC3RSINV __attribute__((section("sfrs")));
extern volatile unsigned int        OC4CON __attribute__((section("sfrs")));
extern volatile unsigned int        OC4CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        OC4CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        OC4CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        OC4R __attribute__((section("sfrs")));
extern volatile unsigned int        OC4RCLR __attribute__((section("sfrs")));
extern volatile unsigned int        OC4RSET __attribute__((section("sfrs")));
extern volatile unsigned int        OC4RINV __attribute__((section("sfrs")));
extern volatile unsigned int        OC4RS __attribute__((section("sfrs")));
extern volatile unsigned int        OC4RSCLR __attribute__((section("sfrs")));
extern volatile unsigned int        OC4RSSET __attribute__((section("sfrs")));
extern volatile unsigned int        OC4RSINV __attribute__((section("sfrs")));
extern volatile unsigned int        OC5CON __attribute__((section("sfrs")));
extern volatile unsigned int        OC5CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        OC5CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        OC5CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        OC5R __attribute__((section("sfrs")));
extern volatile unsigned int        OC5RCLR __attribute__((section("sfrs")));
extern volatile unsigned int        OC5RSET __attribute__((section("sfrs")));
extern volatile unsigned int        OC5RINV __attribute__((section("sfrs")));
extern volatile unsigned int        OC5RS __attribute__((section("sfrs")));
extern volatile unsigned int        OC5RSCLR __attribute__((section("sfrs")));
extern volatile unsigned int        OC5RSSET __attribute__((section("sfrs")));
extern volatile unsigned int        OC5RSINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ACON __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1CON __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ACONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ACONSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ACONINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ASTAT __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1STAT __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ASTATCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1STATCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ASTATSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1STATSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ASTATINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1STATINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1AADD __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ADD __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1AADDCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ADDCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1AADDSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ADDSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1AADDINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ADDINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1AMSK __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1MSK __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1AMSKCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1MSKCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1AMSKSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1MSKSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1AMSKINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1MSKINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ABRG __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1BRG __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ABRGCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1BRGCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ABRGSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1BRGSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ABRGINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1BRGINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ATRN __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1TRN __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ATRNCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1TRNCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ATRNSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1TRNSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ATRNINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1TRNINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1ARCV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C1RCV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ACON __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2CON __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ACONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ACONSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ACONINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ASTAT __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2STAT __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ASTATCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2STATCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ASTATSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2STATSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ASTATINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2STATINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2AADD __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ADD __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2AADDCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ADDCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2AADDSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ADDSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2AADDINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ADDINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2AMSK __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2MSK __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2AMSKCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2MSKCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2AMSKSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2MSKSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2AMSKINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2MSKINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ABRG __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2BRG __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ABRGCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2BRGCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ABRGSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2BRGSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ABRGINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2BRGINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ATRN __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2TRN __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ATRNCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2TRNCLR __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ATRNSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2TRNSET __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ATRNINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2TRNINV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2ARCV __attribute__((section("sfrs")));
extern volatile unsigned int        I2C2RCV __attribute__((section("sfrs")));
extern volatile unsigned int        SPI1CON __attribute__((section("sfrs")));
extern volatile unsigned int        SPI1CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        SPI1CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        SPI1CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        SPI1STAT __attribute__((section("sfrs")));
extern volatile unsigned int        SPI1STATCLR __attribute__((section("sfrs")));
extern volatile unsigned int        SPI1STATSET __attribute__((section("sfrs")));
extern volatile unsigned int        SPI1STATINV __attribute__((section("sfrs")));
extern volatile unsigned int        SPI1BUF __attribute__((section("sfrs")));
extern volatile unsigned int        SPI1BRG __attribute__((section("sfrs")));
extern volatile unsigned int        SPI1BRGCLR __attribute__((section("sfrs")));
extern volatile unsigned int        SPI1BRGSET __attribute__((section("sfrs")));
extern volatile unsigned int        SPI1BRGINV __attribute__((section("sfrs")));
extern volatile unsigned int        SPI1CON2 __attribute__((section("sfrs")));
extern volatile unsigned int        SPI1CON2CLR __attribute__((section("sfrs")));
extern volatile unsigned int        SPI1CON2SET __attribute__((section("sfrs")));
extern volatile unsigned int        SPI1CON2INV __attribute__((section("sfrs")));
extern volatile unsigned int        SPI2CON __attribute__((section("sfrs")));
extern volatile unsigned int        SPI2CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        SPI2CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        SPI2CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        SPI2STAT __attribute__((section("sfrs")));
extern volatile unsigned int        SPI2STATCLR __attribute__((section("sfrs")));
extern volatile unsigned int        SPI2STATSET __attribute__((section("sfrs")));
extern volatile unsigned int        SPI2STATINV __attribute__((section("sfrs")));
extern volatile unsigned int        SPI2BUF __attribute__((section("sfrs")));
extern volatile unsigned int        SPI2BRG __attribute__((section("sfrs")));
extern volatile unsigned int        SPI2BRGCLR __attribute__((section("sfrs")));
extern volatile unsigned int        SPI2BRGSET __attribute__((section("sfrs")));
extern volatile unsigned int        SPI2BRGINV __attribute__((section("sfrs")));
extern volatile unsigned int        SPI2CON2 __attribute__((section("sfrs")));
extern volatile unsigned int        SPI2CON2CLR __attribute__((section("sfrs")));
extern volatile unsigned int        SPI2CON2SET __attribute__((section("sfrs")));
extern volatile unsigned int        SPI2CON2INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1AMODE __attribute__((section("sfrs")));
extern volatile unsigned int        U1MODE __attribute__((section("sfrs")));
extern volatile unsigned int        U1AMODECLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1MODECLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1AMODESET __attribute__((section("sfrs")));
extern volatile unsigned int        U1MODESET __attribute__((section("sfrs")));
extern volatile unsigned int        U1AMODEINV __attribute__((section("sfrs")));
extern volatile unsigned int        U1MODEINV __attribute__((section("sfrs")));
extern volatile unsigned int        U1ASTA __attribute__((section("sfrs")));
extern volatile unsigned int        U1STA __attribute__((section("sfrs")));
extern volatile unsigned int        U1ASTACLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1STACLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1ASTASET __attribute__((section("sfrs")));
extern volatile unsigned int        U1STASET __attribute__((section("sfrs")));
extern volatile unsigned int        U1ASTAINV __attribute__((section("sfrs")));
extern volatile unsigned int        U1STAINV __attribute__((section("sfrs")));
extern volatile unsigned int        U1ATXREG __attribute__((section("sfrs")));
extern volatile unsigned int        U1TXREG __attribute__((section("sfrs")));
extern volatile unsigned int        U1ARXREG __attribute__((section("sfrs")));
extern volatile unsigned int        U1RXREG __attribute__((section("sfrs")));
extern volatile unsigned int        U1ABRG __attribute__((section("sfrs")));
extern volatile unsigned int        U1BRG __attribute__((section("sfrs")));
extern volatile unsigned int        U1ABRGCLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1BRGCLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1ABRGSET __attribute__((section("sfrs")));
extern volatile unsigned int        U1BRGSET __attribute__((section("sfrs")));
extern volatile unsigned int        U1ABRGINV __attribute__((section("sfrs")));
extern volatile unsigned int        U1BRGINV __attribute__((section("sfrs")));
extern volatile unsigned int        U2MODE __attribute__((section("sfrs")));
extern volatile unsigned int        U3AMODE __attribute__((section("sfrs")));
extern volatile unsigned int        U2MODECLR __attribute__((section("sfrs")));
extern volatile unsigned int        U3AMODECLR __attribute__((section("sfrs")));
extern volatile unsigned int        U2MODESET __attribute__((section("sfrs")));
extern volatile unsigned int        U3AMODESET __attribute__((section("sfrs")));
extern volatile unsigned int        U2MODEINV __attribute__((section("sfrs")));
extern volatile unsigned int        U3AMODEINV __attribute__((section("sfrs")));
extern volatile unsigned int        U2STA __attribute__((section("sfrs")));
extern volatile unsigned int        U3ASTA __attribute__((section("sfrs")));
extern volatile unsigned int        U2STACLR __attribute__((section("sfrs")));
extern volatile unsigned int        U3ASTACLR __attribute__((section("sfrs")));
extern volatile unsigned int        U2STASET __attribute__((section("sfrs")));
extern volatile unsigned int        U3ASTASET __attribute__((section("sfrs")));
extern volatile unsigned int        U2STAINV __attribute__((section("sfrs")));
extern volatile unsigned int        U3ASTAINV __attribute__((section("sfrs")));
extern volatile unsigned int        U2TXREG __attribute__((section("sfrs")));
extern volatile unsigned int        U3ATXREG __attribute__((section("sfrs")));
extern volatile unsigned int        U2RXREG __attribute__((section("sfrs")));
extern volatile unsigned int        U3ARXREG __attribute__((section("sfrs")));
extern volatile unsigned int        U2BRG __attribute__((section("sfrs")));
extern volatile unsigned int        U3ABRG __attribute__((section("sfrs")));
extern volatile unsigned int        U2BRGCLR __attribute__((section("sfrs")));
extern volatile unsigned int        U3ABRGCLR __attribute__((section("sfrs")));
extern volatile unsigned int        U2BRGSET __attribute__((section("sfrs")));
extern volatile unsigned int        U3ABRGSET __attribute__((section("sfrs")));
extern volatile unsigned int        U2BRGINV __attribute__((section("sfrs")));
extern volatile unsigned int        U3ABRGINV __attribute__((section("sfrs")));
extern volatile unsigned int        PMCON __attribute__((section("sfrs")));
extern volatile unsigned int        PMCONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        PMCONSET __attribute__((section("sfrs")));
extern volatile unsigned int        PMCONINV __attribute__((section("sfrs")));
extern volatile unsigned int        PMMODE __attribute__((section("sfrs")));
extern volatile unsigned int        PMMODECLR __attribute__((section("sfrs")));
extern volatile unsigned int        PMMODESET __attribute__((section("sfrs")));
extern volatile unsigned int        PMMODEINV __attribute__((section("sfrs")));
extern volatile unsigned int        PMADDR __attribute__((section("sfrs")));
extern volatile unsigned int        PMADDRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        PMADDRSET __attribute__((section("sfrs")));
extern volatile unsigned int        PMADDRINV __attribute__((section("sfrs")));
extern volatile unsigned int        PMDOUT __attribute__((section("sfrs")));
extern volatile unsigned int        PMDOUTCLR __attribute__((section("sfrs")));
extern volatile unsigned int        PMDOUTSET __attribute__((section("sfrs")));
extern volatile unsigned int        PMDOUTINV __attribute__((section("sfrs")));
extern volatile unsigned int        PMDIN __attribute__((section("sfrs")));
extern volatile unsigned int        PMDINCLR __attribute__((section("sfrs")));
extern volatile unsigned int        PMDINSET __attribute__((section("sfrs")));
extern volatile unsigned int        PMDININV __attribute__((section("sfrs")));
extern volatile unsigned int        PMAEN __attribute__((section("sfrs")));
extern volatile unsigned int        PMAENCLR __attribute__((section("sfrs")));
extern volatile unsigned int        PMAENSET __attribute__((section("sfrs")));
extern volatile unsigned int        PMAENINV __attribute__((section("sfrs")));
extern volatile unsigned int        PMSTAT __attribute__((section("sfrs")));
extern volatile unsigned int        PMSTATCLR __attribute__((section("sfrs")));
extern volatile unsigned int        PMSTATSET __attribute__((section("sfrs")));
extern volatile unsigned int        PMSTATINV __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CON1 __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CON1CLR __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CON1SET __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CON1INV __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CON2 __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CON2CLR __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CON2SET __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CON2INV __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CON3 __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CON3CLR __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CON3SET __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CON3INV __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CHS __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CHSCLR __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CHSSET __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CHSINV __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CSSL __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CSSLCLR __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CSSLSET __attribute__((section("sfrs")));
extern volatile unsigned int        AD1CSSLINV __attribute__((section("sfrs")));
extern volatile unsigned int        ADC1BUF0 __attribute__((section("sfrs")));
extern volatile unsigned int        ADC1BUF1 __attribute__((section("sfrs")));
extern volatile unsigned int        ADC1BUF2 __attribute__((section("sfrs")));
extern volatile unsigned int        ADC1BUF3 __attribute__((section("sfrs")));
extern volatile unsigned int        ADC1BUF4 __attribute__((section("sfrs")));
extern volatile unsigned int        ADC1BUF5 __attribute__((section("sfrs")));
extern volatile unsigned int        ADC1BUF6 __attribute__((section("sfrs")));
extern volatile unsigned int        ADC1BUF7 __attribute__((section("sfrs")));
extern volatile unsigned int        ADC1BUF8 __attribute__((section("sfrs")));
extern volatile unsigned int        ADC1BUF9 __attribute__((section("sfrs")));
extern volatile unsigned int        ADC1BUFA __attribute__((section("sfrs")));
extern volatile unsigned int        ADC1BUFB __attribute__((section("sfrs")));
extern volatile unsigned int        ADC1BUFC __attribute__((section("sfrs")));
extern volatile unsigned int        ADC1BUFD __attribute__((section("sfrs")));
extern volatile unsigned int        ADC1BUFE __attribute__((section("sfrs")));
extern volatile unsigned int        ADC1BUFF __attribute__((section("sfrs")));
extern volatile unsigned int        CVRCON __attribute__((section("sfrs")));
extern volatile unsigned int        CVRCONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        CVRCONSET __attribute__((section("sfrs")));
extern volatile unsigned int        CVRCONINV __attribute__((section("sfrs")));
extern volatile unsigned int        CM1CON __attribute__((section("sfrs")));
extern volatile unsigned int        CM1CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        CM1CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        CM1CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        CM2CON __attribute__((section("sfrs")));
extern volatile unsigned int        CM2CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        CM2CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        CM2CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        CM3CON __attribute__((section("sfrs")));
extern volatile unsigned int        CM3CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        CM3CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        CM3CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        CMSTAT __attribute__((section("sfrs")));
extern volatile unsigned int        CMSTATCLR __attribute__((section("sfrs")));
extern volatile unsigned int        CMSTATSET __attribute__((section("sfrs")));
extern volatile unsigned int        CMSTATINV __attribute__((section("sfrs")));
extern volatile unsigned int        CTMUCON __attribute__((section("sfrs")));
extern volatile unsigned int        CTMUCONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        CTMUCONSET __attribute__((section("sfrs")));
extern volatile unsigned int        CTMUCONINV __attribute__((section("sfrs")));
extern volatile unsigned int        OSCCON __attribute__((section("sfrs")));
extern volatile unsigned int        OSCCONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        OSCCONSET __attribute__((section("sfrs")));
extern volatile unsigned int        OSCCONINV __attribute__((section("sfrs")));
extern volatile unsigned int        OSCTUN __attribute__((section("sfrs")));
extern volatile unsigned int        OSCTUNCLR __attribute__((section("sfrs")));
extern volatile unsigned int        OSCTUNSET __attribute__((section("sfrs")));
extern volatile unsigned int        OSCTUNINV __attribute__((section("sfrs")));
extern volatile unsigned int        REFOCON __attribute__((section("sfrs")));
extern volatile unsigned int        REFOCONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        REFOCONSET __attribute__((section("sfrs")));
extern volatile unsigned int        REFOCONINV __attribute__((section("sfrs")));
extern volatile unsigned int        REFOTRIM __attribute__((section("sfrs")));
extern volatile unsigned int        REFOTRIMCLR __attribute__((section("sfrs")));
extern volatile unsigned int        REFOTRIMSET __attribute__((section("sfrs")));
extern volatile unsigned int        REFOTRIMINV __attribute__((section("sfrs")));
extern volatile unsigned int        CFGCON __attribute__((section("sfrs")));
extern volatile unsigned int        DDPCON __attribute__((section("sfrs")));
extern volatile unsigned int        DEVID __attribute__((section("sfrs")));
extern volatile unsigned int        SYSKEY __attribute__((section("sfrs")));
extern volatile unsigned int        SYSKEYCLR __attribute__((section("sfrs")));
extern volatile unsigned int        SYSKEYSET __attribute__((section("sfrs")));
extern volatile unsigned int        SYSKEYINV __attribute__((section("sfrs")));
extern volatile unsigned int        PMD1 __attribute__((section("sfrs")));
extern volatile unsigned int        PMD1CLR __attribute__((section("sfrs")));
extern volatile unsigned int        PMD1SET __attribute__((section("sfrs")));
extern volatile unsigned int        PMD1INV __attribute__((section("sfrs")));
extern volatile unsigned int        PMD2 __attribute__((section("sfrs")));
extern volatile unsigned int        PMD2CLR __attribute__((section("sfrs")));
extern volatile unsigned int        PMD2SET __attribute__((section("sfrs")));
extern volatile unsigned int        PMD2INV __attribute__((section("sfrs")));
extern volatile unsigned int        PMD3 __attribute__((section("sfrs")));
extern volatile unsigned int        PMD3CLR __attribute__((section("sfrs")));
extern volatile unsigned int        PMD3SET __attribute__((section("sfrs")));
extern volatile unsigned int        PMD3INV __attribute__((section("sfrs")));
extern volatile unsigned int        PMD4 __attribute__((section("sfrs")));
extern volatile unsigned int        PMD4CLR __attribute__((section("sfrs")));
extern volatile unsigned int        PMD4SET __attribute__((section("sfrs")));
extern volatile unsigned int        PMD4INV __attribute__((section("sfrs")));
extern volatile unsigned int        PMD5 __attribute__((section("sfrs")));
extern volatile unsigned int        PMD5CLR __attribute__((section("sfrs")));
extern volatile unsigned int        PMD5SET __attribute__((section("sfrs")));
extern volatile unsigned int        PMD5INV __attribute__((section("sfrs")));
extern volatile unsigned int        PMD6 __attribute__((section("sfrs")));
extern volatile unsigned int        PMD6CLR __attribute__((section("sfrs")));
extern volatile unsigned int        PMD6SET __attribute__((section("sfrs")));
extern volatile unsigned int        PMD6INV __attribute__((section("sfrs")));
extern volatile unsigned int        NVMCON __attribute__((section("sfrs")));
extern volatile unsigned int        NVMCONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        NVMCONSET __attribute__((section("sfrs")));
extern volatile unsigned int        NVMCONINV __attribute__((section("sfrs")));
extern volatile unsigned int        NVMKEY __attribute__((section("sfrs")));
extern volatile unsigned int        NVMADDR __attribute__((section("sfrs")));
extern volatile unsigned int        NVMADDRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        NVMADDRSET __attribute__((section("sfrs")));
extern volatile unsigned int        NVMADDRINV __attribute__((section("sfrs")));
extern volatile unsigned int        NVMDATA __attribute__((section("sfrs")));
extern volatile unsigned int        NVMSRCADDR __attribute__((section("sfrs")));
extern volatile unsigned int        RCON __attribute__((section("sfrs")));
extern volatile unsigned int        RCONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        RCONSET __attribute__((section("sfrs")));
extern volatile unsigned int        RCONINV __attribute__((section("sfrs")));
extern volatile unsigned int        RSWRST __attribute__((section("sfrs")));
extern volatile unsigned int        RSWRSTCLR __attribute__((section("sfrs")));
extern volatile unsigned int        RSWRSTSET __attribute__((section("sfrs")));
extern volatile unsigned int        RSWRSTINV __attribute__((section("sfrs")));
extern volatile unsigned int        INT1R __attribute__((section("sfrs")));
extern volatile unsigned int        INT2R __attribute__((section("sfrs")));
extern volatile unsigned int        INT3R __attribute__((section("sfrs")));
extern volatile unsigned int        INT4R __attribute__((section("sfrs")));
extern volatile unsigned int        T2CKR __attribute__((section("sfrs")));
extern volatile unsigned int        T3CKR __attribute__((section("sfrs")));
extern volatile unsigned int        T4CKR __attribute__((section("sfrs")));
extern volatile unsigned int        T5CKR __attribute__((section("sfrs")));
extern volatile unsigned int        IC1R __attribute__((section("sfrs")));
extern volatile unsigned int        IC2R __attribute__((section("sfrs")));
extern volatile unsigned int        IC3R __attribute__((section("sfrs")));
extern volatile unsigned int        IC4R __attribute__((section("sfrs")));
extern volatile unsigned int        IC5R __attribute__((section("sfrs")));
extern volatile unsigned int        OCFAR __attribute__((section("sfrs")));
extern volatile unsigned int        OCFBR __attribute__((section("sfrs")));
extern volatile unsigned int        U1RXR __attribute__((section("sfrs")));
extern volatile unsigned int        U1CTSR __attribute__((section("sfrs")));
extern volatile unsigned int        U2RXR __attribute__((section("sfrs")));
extern volatile unsigned int        U2CTSR __attribute__((section("sfrs")));
extern volatile unsigned int        SDI1R __attribute__((section("sfrs")));
extern volatile unsigned int        SS1R __attribute__((section("sfrs")));
extern volatile unsigned int        SDI2R __attribute__((section("sfrs")));
extern volatile unsigned int        SS2R __attribute__((section("sfrs")));
extern volatile unsigned int        REFCLKIR __attribute__((section("sfrs")));
extern volatile unsigned int        RPA0R __attribute__((section("sfrs")));
extern volatile unsigned int        RPA1R __attribute__((section("sfrs")));
extern volatile unsigned int        RPA2R __attribute__((section("sfrs")));
extern volatile unsigned int        RPA3R __attribute__((section("sfrs")));
extern volatile unsigned int        RPA4R __attribute__((section("sfrs")));
extern volatile unsigned int        RPA8R __attribute__((section("sfrs")));
extern volatile unsigned int        RPA9R __attribute__((section("sfrs")));
extern volatile unsigned int        RPB0R __attribute__((section("sfrs")));
extern volatile unsigned int        RPB1R __attribute__((section("sfrs")));
extern volatile unsigned int        RPB2R __attribute__((section("sfrs")));
extern volatile unsigned int        RPB3R __attribute__((section("sfrs")));
extern volatile unsigned int        RPB4R __attribute__((section("sfrs")));
extern volatile unsigned int        RPB5R __attribute__((section("sfrs")));
extern volatile unsigned int        RPB7R __attribute__((section("sfrs")));
extern volatile unsigned int        RPB8R __attribute__((section("sfrs")));
extern volatile unsigned int        RPB9R __attribute__((section("sfrs")));
extern volatile unsigned int        RPB10R __attribute__((section("sfrs")));
extern volatile unsigned int        RPB11R __attribute__((section("sfrs")));
extern volatile unsigned int        RPB13R __attribute__((section("sfrs")));
extern volatile unsigned int        RPB14R __attribute__((section("sfrs")));
extern volatile unsigned int        RPB15R __attribute__((section("sfrs")));
extern volatile unsigned int        RPC0R __attribute__((section("sfrs")));
extern volatile unsigned int        RPC1R __attribute__((section("sfrs")));
extern volatile unsigned int        RPC2R __attribute__((section("sfrs")));
extern volatile unsigned int        RPC3R __attribute__((section("sfrs")));
extern volatile unsigned int        RPC4R __attribute__((section("sfrs")));
extern volatile unsigned int        RPC5R __attribute__((section("sfrs")));
extern volatile unsigned int        RPC6R __attribute__((section("sfrs")));
extern volatile unsigned int        RPC7R __attribute__((section("sfrs")));
extern volatile unsigned int        RPC8R __attribute__((section("sfrs")));
extern volatile unsigned int        RPC9R __attribute__((section("sfrs")));
extern volatile unsigned int        INTCON __attribute__((section("sfrs")));
extern volatile unsigned int        INTCONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        INTCONSET __attribute__((section("sfrs")));
extern volatile unsigned int        INTCONINV __attribute__((section("sfrs")));
extern volatile unsigned int        INTSTAT __attribute__((section("sfrs")));
extern volatile unsigned int        IPTMR __attribute__((section("sfrs")));
extern volatile unsigned int        IPTMRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        IPTMRSET __attribute__((section("sfrs")));
extern volatile unsigned int        IPTMRINV __attribute__((section("sfrs")));
extern volatile unsigned int        IFS0 __attribute__((section("sfrs")));
extern volatile unsigned int        IFS0CLR __attribute__((section("sfrs")));
extern volatile unsigned int        IFS0SET __attribute__((section("sfrs")));
extern volatile unsigned int        IFS0INV __attribute__((section("sfrs")));
extern volatile unsigned int        IFS1 __attribute__((section("sfrs")));
extern volatile unsigned int        IFS1CLR __attribute__((section("sfrs")));
extern volatile unsigned int        IFS1SET __attribute__((section("sfrs")));
extern volatile unsigned int        IFS1INV __attribute__((section("sfrs")));
extern volatile unsigned int        IEC0 __attribute__((section("sfrs")));
extern volatile unsigned int        IEC0CLR __attribute__((section("sfrs")));
extern volatile unsigned int        IEC0SET __attribute__((section("sfrs")));
extern volatile unsigned int        IEC0INV __attribute__((section("sfrs")));
extern volatile unsigned int        IEC1 __attribute__((section("sfrs")));
extern volatile unsigned int        IEC1CLR __attribute__((section("sfrs")));
extern volatile unsigned int        IEC1SET __attribute__((section("sfrs")));
extern volatile unsigned int        IEC1INV __attribute__((section("sfrs")));
extern volatile unsigned int        IPC0 __attribute__((section("sfrs")));
extern volatile unsigned int        IPC0CLR __attribute__((section("sfrs")));
extern volatile unsigned int        IPC0SET __attribute__((section("sfrs")));
extern volatile unsigned int        IPC0INV __attribute__((section("sfrs")));
extern volatile unsigned int        IPC1 __attribute__((section("sfrs")));
extern volatile unsigned int        IPC1CLR __attribute__((section("sfrs")));
extern volatile unsigned int        IPC1SET __attribute__((section("sfrs")));
extern volatile unsigned int        IPC1INV __attribute__((section("sfrs")));
extern volatile unsigned int        IPC2 __attribute__((section("sfrs")));
extern volatile unsigned int        IPC2CLR __attribute__((section("sfrs")));
extern volatile unsigned int        IPC2SET __attribute__((section("sfrs")));
extern volatile unsigned int        IPC2INV __attribute__((section("sfrs")));
extern volatile unsigned int        IPC3 __attribute__((section("sfrs")));
extern volatile unsigned int        IPC3CLR __attribute__((section("sfrs")));
extern volatile unsigned int        IPC3SET __attribute__((section("sfrs")));
extern volatile unsigned int        IPC3INV __attribute__((section("sfrs")));
extern volatile unsigned int        IPC4 __attribute__((section("sfrs")));
extern volatile unsigned int        IPC4CLR __attribute__((section("sfrs")));
extern volatile unsigned int        IPC4SET __attribute__((section("sfrs")));
extern volatile unsigned int        IPC4INV __attribute__((section("sfrs")));
extern volatile unsigned int        IPC5 __attribute__((section("sfrs")));
extern volatile unsigned int        IPC5CLR __attribute__((section("sfrs")));
extern volatile unsigned int        IPC5SET __attribute__((section("sfrs")));
extern volatile unsigned int        IPC5INV __attribute__((section("sfrs")));
extern volatile unsigned int        IPC6 __attribute__((section("sfrs")));
extern volatile unsigned int        IPC6CLR __attribute__((section("sfrs")));
extern volatile unsigned int        IPC6SET __attribute__((section("sfrs")));
extern volatile unsigned int        IPC6INV __attribute__((section("sfrs")));
extern volatile unsigned int        IPC7 __attribute__((section("sfrs")));
extern volatile unsigned int        IPC7CLR __attribute__((section("sfrs")));
extern volatile unsigned int        IPC7SET __attribute__((section("sfrs")));
extern volatile unsigned int        IPC7INV __attribute__((section("sfrs")));
extern volatile unsigned int        IPC8 __attribute__((section("sfrs")));
extern volatile unsigned int        IPC8CLR __attribute__((section("sfrs")));
extern volatile unsigned int        IPC8SET __attribute__((section("sfrs")));
extern volatile unsigned int        IPC8INV __attribute__((section("sfrs")));
extern volatile unsigned int        IPC9 __attribute__((section("sfrs")));
extern volatile unsigned int        IPC9CLR __attribute__((section("sfrs")));
extern volatile unsigned int        IPC9SET __attribute__((section("sfrs")));
extern volatile unsigned int        IPC9INV __attribute__((section("sfrs")));
extern volatile unsigned int        IPC10 __attribute__((section("sfrs")));
extern volatile unsigned int        IPC10CLR __attribute__((section("sfrs")));
extern volatile unsigned int        IPC10SET __attribute__((section("sfrs")));
extern volatile unsigned int        IPC10INV __attribute__((section("sfrs")));
extern volatile unsigned int        BMXCON __attribute__((section("sfrs")));
extern volatile unsigned int        BMXCONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        BMXCONSET __attribute__((section("sfrs")));
extern volatile unsigned int        BMXCONINV __attribute__((section("sfrs")));
extern volatile unsigned int        BMXDKPBA __attribute__((section("sfrs")));
extern volatile unsigned int        BMXDKPBACLR __attribute__((section("sfrs")));
extern volatile unsigned int        BMXDKPBASET __attribute__((section("sfrs")));
extern volatile unsigned int        BMXDKPBAINV __attribute__((section("sfrs")));
extern volatile unsigned int        BMXDUDBA __attribute__((section("sfrs")));
extern volatile unsigned int        BMXDUDBACLR __attribute__((section("sfrs")));
extern volatile unsigned int        BMXDUDBASET __attribute__((section("sfrs")));
extern volatile unsigned int        BMXDUDBAINV __attribute__((section("sfrs")));
extern volatile unsigned int        BMXDUPBA __attribute__((section("sfrs")));
extern volatile unsigned int        BMXDUPBACLR __attribute__((section("sfrs")));
extern volatile unsigned int        BMXDUPBASET __attribute__((section("sfrs")));
extern volatile unsigned int        BMXDUPBAINV __attribute__((section("sfrs")));
extern volatile unsigned int        BMXDRMSZ __attribute__((section("sfrs")));
extern volatile unsigned int        BMXPUPBA __attribute__((section("sfrs")));
extern volatile unsigned int        BMXPUPBACLR __attribute__((section("sfrs")));
extern volatile unsigned int        BMXPUPBASET __attribute__((section("sfrs")));
extern volatile unsigned int        BMXPUPBAINV __attribute__((section("sfrs")));
extern volatile unsigned int        BMXPFMSZ __attribute__((section("sfrs")));
extern volatile unsigned int        BMXBOOTSZ __attribute__((section("sfrs")));
extern volatile unsigned int        DMACON __attribute__((section("sfrs")));
extern volatile unsigned int        DMACONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DMACONSET __attribute__((section("sfrs")));
extern volatile unsigned int        DMACONINV __attribute__((section("sfrs")));
extern volatile unsigned int        DMASTAT __attribute__((section("sfrs")));
extern volatile unsigned int        DMASTATCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DMASTATSET __attribute__((section("sfrs")));
extern volatile unsigned int        DMASTATINV __attribute__((section("sfrs")));
extern volatile unsigned int        DMAADDR __attribute__((section("sfrs")));
extern volatile unsigned int        DMAADDRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DMAADDRSET __attribute__((section("sfrs")));
extern volatile unsigned int        DMAADDRINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCRCCON __attribute__((section("sfrs")));
extern volatile unsigned int        DCRCCONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCRCCONSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCRCCONINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCRCDATA __attribute__((section("sfrs")));
extern volatile unsigned int        DCRCDATACLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCRCDATASET __attribute__((section("sfrs")));
extern volatile unsigned int        DCRCDATAINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCRCXOR __attribute__((section("sfrs")));
extern volatile unsigned int        DCRCXORCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCRCXORSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCRCXORINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0CON __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0ECON __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0ECONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0ECONSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0ECONINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0INT __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0INTCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0INTSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0INTINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0SSA __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0SSACLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0SSASET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0SSAINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0DSA __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0DSACLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0DSASET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0DSAINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0SSIZ __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0SSIZCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0SSIZSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0SSIZINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0DSIZ __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0DSIZCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0DSIZSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0DSIZINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0SPTR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0SPTRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0SPTRSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0SPTRINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0DPTR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0DPTRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0DPTRSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0DPTRINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0CSIZ __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0CSIZCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0CSIZSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0CSIZINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0CPTR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0CPTRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0CPTRSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0CPTRINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0DAT __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0DATCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0DATSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH0DATINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1CON __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1ECON __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1ECONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1ECONSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1ECONINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1INT __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1INTCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1INTSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1INTINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1SSA __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1SSACLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1SSASET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1SSAINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1DSA __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1DSACLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1DSASET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1DSAINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1SSIZ __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1SSIZCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1SSIZSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1SSIZINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1DSIZ __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1DSIZCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1DSIZSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1DSIZINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1SPTR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1SPTRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1SPTRSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1SPTRINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1DPTR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1DPTRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1DPTRSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1DPTRINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1CSIZ __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1CSIZCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1CSIZSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1CSIZINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1CPTR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1CPTRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1CPTRSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1CPTRINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1DAT __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1DATCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1DATSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH1DATINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2CON __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2ECON __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2ECONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2ECONSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2ECONINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2INT __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2INTCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2INTSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2INTINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2SSA __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2SSACLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2SSASET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2SSAINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2DSA __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2DSACLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2DSASET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2DSAINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2SSIZ __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2SSIZCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2SSIZSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2SSIZINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2DSIZ __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2DSIZCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2DSIZSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2DSIZINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2SPTR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2SPTRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2SPTRSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2SPTRINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2DPTR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2DPTRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2DPTRSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2DPTRINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2CSIZ __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2CSIZCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2CSIZSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2CSIZINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2CPTR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2CPTRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2CPTRSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2CPTRINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2DAT __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2DATCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2DATSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH2DATINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3CON __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3ECON __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3ECONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3ECONSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3ECONINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3INT __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3INTCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3INTSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3INTINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3SSA __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3SSACLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3SSASET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3SSAINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3DSA __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3DSACLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3DSASET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3DSAINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3SSIZ __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3SSIZCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3SSIZSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3SSIZINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3DSIZ __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3DSIZCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3DSIZSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3DSIZINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3SPTR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3SPTRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3SPTRSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3SPTRINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3DPTR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3DPTRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3DPTRSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3DPTRINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3CSIZ __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3CSIZCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3CSIZSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3CSIZINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3CPTR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3CPTRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3CPTRSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3CPTRINV __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3DAT __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3DATCLR __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3DATSET __attribute__((section("sfrs")));
extern volatile unsigned int        DCH3DATINV __attribute__((section("sfrs")));
extern volatile unsigned int        U1OTGIR __attribute__((section("sfrs")));
extern volatile unsigned int        U1OTGIRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1OTGIE __attribute__((section("sfrs")));
extern volatile unsigned int        U1OTGIECLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1OTGIESET __attribute__((section("sfrs")));
extern volatile unsigned int        U1OTGIEINV __attribute__((section("sfrs")));
extern volatile unsigned int        U1OTGSTAT __attribute__((section("sfrs")));
extern volatile unsigned int        U1OTGCON __attribute__((section("sfrs")));
extern volatile unsigned int        U1OTGCONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1OTGCONSET __attribute__((section("sfrs")));
extern volatile unsigned int        U1OTGCONINV __attribute__((section("sfrs")));
extern volatile unsigned int        U1PWRC __attribute__((section("sfrs")));
extern volatile unsigned int        U1PWRCCLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1PWRCSET __attribute__((section("sfrs")));
extern volatile unsigned int        U1PWRCINV __attribute__((section("sfrs")));
extern volatile unsigned int        U1IR __attribute__((section("sfrs")));
extern volatile unsigned int        U1IRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1IE __attribute__((section("sfrs")));
extern volatile unsigned int        U1IECLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1IESET __attribute__((section("sfrs")));
extern volatile unsigned int        U1IEINV __attribute__((section("sfrs")));
extern volatile unsigned int        U1EIR __attribute__((section("sfrs")));
extern volatile unsigned int        U1EIRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1EIE __attribute__((section("sfrs")));
extern volatile unsigned int        U1EIECLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1EIESET __attribute__((section("sfrs")));
extern volatile unsigned int        U1EIEINV __attribute__((section("sfrs")));
extern volatile unsigned int        U1STAT __attribute__((section("sfrs")));
extern volatile unsigned int        U1CON __attribute__((section("sfrs")));
extern volatile unsigned int        U1CONCLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1CONSET __attribute__((section("sfrs")));
extern volatile unsigned int        U1CONINV __attribute__((section("sfrs")));
extern volatile unsigned int        U1ADDR __attribute__((section("sfrs")));
extern volatile unsigned int        U1ADDRCLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1ADDRSET __attribute__((section("sfrs")));
extern volatile unsigned int        U1ADDRINV __attribute__((section("sfrs")));
extern volatile unsigned int        U1BDTP1 __attribute__((section("sfrs")));
extern volatile unsigned int        U1BDTP1CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1BDTP1SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1BDTP1INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1FRML __attribute__((section("sfrs")));
extern volatile unsigned int        U1FRMH __attribute__((section("sfrs")));
extern volatile unsigned int        U1TOK __attribute__((section("sfrs")));
extern volatile unsigned int        U1TOKCLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1TOKSET __attribute__((section("sfrs")));
extern volatile unsigned int        U1TOKINV __attribute__((section("sfrs")));
extern volatile unsigned int        U1SOF __attribute__((section("sfrs")));
extern volatile unsigned int        U1SOFCLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1SOFSET __attribute__((section("sfrs")));
extern volatile unsigned int        U1SOFINV __attribute__((section("sfrs")));
extern volatile unsigned int        U1BDTP2 __attribute__((section("sfrs")));
extern volatile unsigned int        U1BDTP2CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1BDTP2SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1BDTP2INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1BDTP3 __attribute__((section("sfrs")));
extern volatile unsigned int        U1BDTP3CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1BDTP3SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1BDTP3INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1CNFG1 __attribute__((section("sfrs")));
extern volatile unsigned int        U1CNFG1CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1CNFG1SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1CNFG1INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP0 __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP0CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP0SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP0INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP1 __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP1CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP1SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP1INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP2 __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP2CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP2SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP2INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP3 __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP3CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP3SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP3INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP4 __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP4CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP4SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP4INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP5 __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP5CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP5SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP5INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP6 __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP6CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP6SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP6INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP7 __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP7CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP7SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP7INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP8 __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP8CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP8SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP8INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP9 __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP9CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP9SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP9INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP10 __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP10CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP10SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP10INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP11 __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP11CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP11SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP11INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP12 __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP12CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP12SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP12INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP13 __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP13CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP13SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP13INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP14 __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP14CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP14SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP14INV __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP15 __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP15CLR __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP15SET __attribute__((section("sfrs")));
extern volatile unsigned int        U1EP15INV __attribute__((section("sfrs")));
extern volatile unsigned int        ANSELA __attribute__((section("sfrs")));
extern volatile unsigned int        ANSELACLR __attribute__((section("sfrs")));
extern volatile unsigned int        ANSELASET __attribute__((section("sfrs")));
extern volatile unsigned int        ANSELAINV __attribute__((section("sfrs")));
extern volatile unsigned int        TRISA __attribute__((section("sfrs")));
extern volatile unsigned int        TRISACLR __attribute__((section("sfrs")));
extern volatile unsigned int        TRISASET __attribute__((section("sfrs")));
extern volatile unsigned int        TRISAINV __attribute__((section("sfrs")));
extern volatile unsigned int        PORTA __attribute__((section("sfrs")));
extern volatile unsigned int        PORTACLR __attribute__((section("sfrs")));
extern volatile unsigned int        PORTASET __attribute__((section("sfrs")));
extern volatile unsigned int        PORTAINV __attribute__((section("sfrs")));
extern volatile unsigned int        LATA __attribute__((section("sfrs")));
extern volatile unsigned int        LATACLR __attribute__((section("sfrs")));
extern volatile unsigned int        LATASET __attribute__((section("sfrs")));
extern volatile unsigned int        LATAINV __attribute__((section("sfrs")));
extern volatile unsigned int        ODCA __attribute__((section("sfrs")));
extern volatile unsigned int        ODCACLR __attribute__((section("sfrs")));
extern volatile unsigned int        ODCASET __attribute__((section("sfrs")));
extern volatile unsigned int        ODCAINV __attribute__((section("sfrs")));
extern volatile unsigned int        CNPUA __attribute__((section("sfrs")));
extern volatile unsigned int        CNPUACLR __attribute__((section("sfrs")));
extern volatile unsigned int        CNPUASET __attribute__((section("sfrs")));
extern volatile unsigned int        CNPUAINV __attribute__((section("sfrs")));
extern volatile unsigned int        CNPDA __attribute__((section("sfrs")));
extern volatile unsigned int        CNPDACLR __attribute__((section("sfrs")));
extern volatile unsigned int        CNPDASET __attribute__((section("sfrs")));
extern volatile unsigned int        CNPDAINV __attribute__((section("sfrs")));
extern volatile unsigned int        CNCONA __attribute__((section("sfrs")));
extern volatile unsigned int        CNCONACLR __attribute__((section("sfrs")));
extern volatile unsigned int        CNCONASET __attribute__((section("sfrs")));
extern volatile unsigned int        CNCONAINV __attribute__((section("sfrs")));
extern volatile unsigned int        CNENA __attribute__((section("sfrs")));
extern volatile unsigned int        CNENACLR __attribute__((section("sfrs")));
extern volatile unsigned int        CNENASET __attribute__((section("sfrs")));
extern volatile unsigned int        CNENAINV __attribute__((section("sfrs")));
extern volatile unsigned int        CNSTATA __attribute__((section("sfrs")));
extern volatile unsigned int        CNSTATACLR __attribute__((section("sfrs")));
extern volatile unsigned int        CNSTATASET __attribute__((section("sfrs")));
extern volatile unsigned int        CNSTATAINV __attribute__((section("sfrs")));
extern volatile unsigned int        ANSELB __attribute__((section("sfrs")));
extern volatile unsigned int        ANSELBCLR __attribute__((section("sfrs")));
extern volatile unsigned int        ANSELBSET __attribute__((section("sfrs")));
extern volatile unsigned int        ANSELBINV __attribute__((section("sfrs")));
extern volatile unsigned int        TRISB __attribute__((section("sfrs")));
extern volatile unsigned int        TRISBCLR __attribute__((section("sfrs")));
extern volatile unsigned int        TRISBSET __attribute__((section("sfrs")));
extern volatile unsigned int        TRISBINV __attribute__((section("sfrs")));
extern volatile unsigned int        PORTB __attribute__((section("sfrs")));
extern volatile unsigned int        PORTBCLR __attribute__((section("sfrs")));
extern volatile unsigned int        PORTBSET __attribute__((section("sfrs")));
extern volatile unsigned int        PORTBINV __attribute__((section("sfrs")));
extern volatile unsigned int        LATB __attribute__((section("sfrs")));
extern volatile unsigned int        LATBCLR __attribute__((section("sfrs")));
extern volatile unsigned int        LATBSET __attribute__((section("sfrs")));
extern volatile unsigned int        LATBINV __attribute__((section("sfrs")));
extern volatile unsigned int        ODCB __attribute__((section("sfrs")));
extern volatile unsigned int        ODCBCLR __attribute__((section("sfrs")));
extern volatile unsigned int        ODCBSET __attribute__((section("sfrs")));
extern volatile unsigned int        ODCBINV __attribute__((section("sfrs")));
extern volatile unsigned int        CNPUB __attribute__((section("sfrs")));
extern volatile unsigned int        CNPUBCLR __attribute__((section("sfrs")));
extern volatile unsigned int        CNPUBSET __attribute__((section("sfrs")));
extern volatile unsigned int        CNPUBINV __attribute__((section("sfrs")));
extern volatile unsigned int        CNPDB __attribute__((section("sfrs")));
extern volatile unsigned int        CNPDBCLR __attribute__((section("sfrs")));
extern volatile unsigned int        CNPDBSET __attribute__((section("sfrs")));
extern volatile unsigned int        CNPDBINV __attribute__((section("sfrs")));
extern volatile unsigned int        CNCONB __attribute__((section("sfrs")));
extern volatile unsigned int        CNCONBCLR __attribute__((section("sfrs")));
extern volatile unsigned int        CNCONBSET __attribute__((section("sfrs")));
extern volatile unsigned int        CNCONBINV __attribute__((section("sfrs")));
extern volatile unsigned int        CNENB __attribute__((section("sfrs")));
extern volatile unsigned int        CNENBCLR __attribute__((section("sfrs")));
extern volatile unsigned int        CNENBSET __attribute__((section("sfrs")));
extern volatile unsigned int        CNENBINV __attribute__((section("sfrs")));
extern volatile unsigned int        CNSTATB __attribute__((section("sfrs")));
extern volatile unsigned int        CNSTATBCLR __attribute__((section("sfrs")));
extern volatile unsigned int        CNSTATBSET __attribute__((section("sfrs")));
extern volatile unsigned int        CNSTATBINV __attribute__((section("sfrs")));
extern volatile unsigned int        DEVCFG3 __attribute__((section("sfrs")));
extern volatile unsigned int        DEVCFG2 __attribute__((section("sfrs")));
extern volatile unsigned int        DEVCFG1 __attribute__((section("sfrs")));
extern volatile unsigned int        DEVCFG0 __attribute__((section("sfrs")));
#ifdef __cplusplus
}
#endif

#elif defined (__LANGUAGE_ASSEMBLY__)
  .extern WDTCON           /* 0xBF800000 */
  .extern WDTCONCLR        /* 0xBF800004 */
  .extern WDTCONSET        /* 0xBF800008 */
  .extern WDTCONINV        /* 0xBF80000C */
  .extern RTCCON           /* 0xBF800200 */
  .extern RTCCONCLR        /* 0xBF800204 */
  .extern RTCCONSET        /* 0xBF800208 */
  .extern RTCCONINV        /* 0xBF80020C */
  .extern RTCALRM          /* 0xBF800210 */
  .extern RTCALRMCLR       /* 0xBF800214 */
  .extern RTCALRMSET       /* 0xBF800218 */
  .extern RTCALRMINV       /* 0xBF80021C */
  .extern RTCTIME          /* 0xBF800220 */
  .extern RTCTIMECLR       /* 0xBF800224 */
  .extern RTCTIMESET       /* 0xBF800228 */
  .extern RTCTIMEINV       /* 0xBF80022C */
  .extern RTCDATE          /* 0xBF800230 */
  .extern RTCDATECLR       /* 0xBF800234 */
  .extern RTCDATESET       /* 0xBF800238 */
  .extern RTCDATEINV       /* 0xBF80023C */
  .extern ALRMTIME         /* 0xBF800240 */
  .extern ALRMTIMECLR      /* 0xBF800244 */
  .extern ALRMTIMESET      /* 0xBF800248 */
  .extern ALRMTIMEINV      /* 0xBF80024C */
  .extern ALRMDATE         /* 0xBF800250 */
  .extern ALRMDATECLR      /* 0xBF800254 */
  .extern ALRMDATESET      /* 0xBF800258 */
  .extern ALRMDATEINV      /* 0xBF80025C */
  .extern T1CON            /* 0xBF800600 */
  .extern T1CONCLR         /* 0xBF800604 */
  .extern T1CONSET         /* 0xBF800608 */
  .extern T1CONINV         /* 0xBF80060C */
  .extern TMR1             /* 0xBF800610 */
  .extern TMR1CLR          /* 0xBF800614 */
  .extern TMR1SET          /* 0xBF800618 */
  .extern TMR1INV          /* 0xBF80061C */
  .extern PR1              /* 0xBF800620 */
  .extern PR1CLR           /* 0xBF800624 */
  .extern PR1SET           /* 0xBF800628 */
  .extern PR1INV           /* 0xBF80062C */
  .extern T2CON            /* 0xBF800800 */
  .extern T2CONCLR         /* 0xBF800804 */
  .extern T2CONSET         /* 0xBF800808 */
  .extern T2CONINV         /* 0xBF80080C */
  .extern TMR2             /* 0xBF800810 */
  .extern TMR2CLR          /* 0xBF800814 */
  .extern TMR2SET          /* 0xBF800818 */
  .extern TMR2INV          /* 0xBF80081C */
  .extern PR2              /* 0xBF800820 */
  .extern PR2CLR           /* 0xBF800824 */
  .extern PR2SET           /* 0xBF800828 */
  .extern PR2INV           /* 0xBF80082C */
  .extern T3CON            /* 0xBF800A00 */
  .extern T3CONCLR         /* 0xBF800A04 */
  .extern T3CONSET         /* 0xBF800A08 */
  .extern T3CONINV         /* 0xBF800A0C */
  .extern TMR3             /* 0xBF800A10 */
  .extern TMR3CLR          /* 0xBF800A14 */
  .extern TMR3SET          /* 0xBF800A18 */
  .extern TMR3INV          /* 0xBF800A1C */
  .extern PR3              /* 0xBF800A20 */
  .extern PR3CLR           /* 0xBF800A24 */
  .extern PR3SET           /* 0xBF800A28 */
  .extern PR3INV           /* 0xBF800A2C */
  .extern T4CON            /* 0xBF800C00 */
  .extern T4CONCLR         /* 0xBF800C04 */
  .extern T4CONSET         /* 0xBF800C08 */
  .extern T4CONINV         /* 0xBF800C0C */
  .extern TMR4             /* 0xBF800C10 */
  .extern TMR4CLR          /* 0xBF800C14 */
  .extern TMR4SET          /* 0xBF800C18 */
  .extern TMR4INV          /* 0xBF800C1C */
  .extern PR4              /* 0xBF800C20 */
  .extern PR4CLR           /* 0xBF800C24 */
  .extern PR4SET           /* 0xBF800C28 */
  .extern PR4INV           /* 0xBF800C2C */
  .extern T5CON            /* 0xBF800E00 */
  .extern T5CONCLR         /* 0xBF800E04 */
  .extern T5CONSET         /* 0xBF800E08 */
  .extern T5CONINV         /* 0xBF800E0C */
  .extern TMR5             /* 0xBF800E10 */
  .extern TMR5CLR          /* 0xBF800E14 */
  .extern TMR5SET          /* 0xBF800E18 */
  .extern TMR5INV          /* 0xBF800E1C */
  .extern PR5              /* 0xBF800E20 */
  .extern PR5CLR           /* 0xBF800E24 */
  .extern PR5SET           /* 0xBF800E28 */
  .extern PR5INV           /* 0xBF800E2C */
  .extern IC1CON           /* 0xBF802000 */
  .extern IC1CONCLR        /* 0xBF802004 */
  .extern IC1CONSET        /* 0xBF802008 */
  .extern IC1CONINV        /* 0xBF80200C */
  .extern IC1BUF           /* 0xBF802010 */
  .extern IC2CON           /* 0xBF802200 */
  .extern IC2CONCLR        /* 0xBF802204 */
  .extern IC2CONSET        /* 0xBF802208 */
  .extern IC2CONINV        /* 0xBF80220C */
  .extern IC2BUF           /* 0xBF802210 */
  .extern IC3CON           /* 0xBF802400 */
  .extern IC3CONCLR        /* 0xBF802404 */
  .extern IC3CONSET        /* 0xBF802408 */
  .extern IC3CONINV        /* 0xBF80240C */
  .extern IC3BUF           /* 0xBF802410 */
  .extern IC4CON           /* 0xBF802600 */
  .extern IC4CONCLR        /* 0xBF802604 */
  .extern IC4CONSET        /* 0xBF802608 */
  .extern IC4CONINV        /* 0xBF80260C */
  .extern IC4BUF           /* 0xBF802610 */
  .extern IC5CON           /* 0xBF802800 */
  .extern IC5CONCLR        /* 0xBF802804 */
  .extern IC5CONSET        /* 0xBF802808 */
  .extern IC5CONINV        /* 0xBF80280C */
  .extern IC5BUF           /* 0xBF802810 */
  .extern OC1CON           /* 0xBF803000 */
  .extern OC1CONCLR        /* 0xBF803004 */
  .extern OC1CONSET        /* 0xBF803008 */
  .extern OC1CONINV        /* 0xBF80300C */
  .extern OC1R             /* 0xBF803010 */
  .extern OC1RCLR          /* 0xBF803014 */
  .extern OC1RSET          /* 0xBF803018 */
  .extern OC1RINV          /* 0xBF80301C */
  .extern OC1RS            /* 0xBF803020 */
  .extern OC1RSCLR         /* 0xBF803024 */
  .extern OC1RSSET         /* 0xBF803028 */
  .extern OC1RSINV         /* 0xBF80302C */
  .extern OC2CON           /* 0xBF803200 */
  .extern OC2CONCLR        /* 0xBF803204 */
  .extern OC2CONSET        /* 0xBF803208 */
  .extern OC2CONINV        /* 0xBF80320C */
  .extern OC2R             /* 0xBF803210 */
  .extern OC2RCLR          /* 0xBF803214 */
  .extern OC2RSET          /* 0xBF803218 */
  .extern OC2RINV          /* 0xBF80321C */
  .extern OC2RS            /* 0xBF803220 */
  .extern OC2RSCLR         /* 0xBF803224 */
  .extern OC2RSSET         /* 0xBF803228 */
  .extern OC2RSINV         /* 0xBF80322C */
  .extern OC3CON           /* 0xBF803400 */
  .extern OC3CONCLR        /* 0xBF803404 */
  .extern OC3CONSET        /* 0xBF803408 */
  .extern OC3CONINV        /* 0xBF80340C */
  .extern OC3R             /* 0xBF803410 */
  .extern OC3RCLR          /* 0xBF803414 */
  .extern OC3RSET          /* 0xBF803418 */
  .extern OC3RINV          /* 0xBF80341C */
  .extern OC3RS            /* 0xBF803420 */
  .extern OC3RSCLR         /* 0xBF803424 */
  .extern OC3RSSET         /* 0xBF803428 */
  .extern OC3RSINV         /* 0xBF80342C */
  .extern OC4CON           /* 0xBF803600 */
  .extern OC4CONCLR        /* 0xBF803604 */
  .extern OC4CONSET        /* 0xBF803608 */
  .extern OC4CONINV        /* 0xBF80360C */
  .extern OC4R             /* 0xBF803610 */
  .extern OC4RCLR          /* 0xBF803614 */
  .extern OC4RSET          /* 0xBF803618 */
  .extern OC4RINV          /* 0xBF80361C */
  .extern OC4RS            /* 0xBF803620 */
  .extern OC4RSCLR         /* 0xBF803624 */
  .extern OC4RSSET         /* 0xBF803628 */
  .extern OC4RSINV         /* 0xBF80362C */
  .extern OC5CON           /* 0xBF803800 */
  .extern OC5CONCLR        /* 0xBF803804 */
  .extern OC5CONSET        /* 0xBF803808 */
  .extern OC5CONINV        /* 0xBF80380C */
  .extern OC5R             /* 0xBF803810 */
  .extern OC5RCLR          /* 0xBF803814 */
  .extern OC5RSET          /* 0xBF803818 */
  .extern OC5RINV          /* 0xBF80381C */
  .extern OC5RS            /* 0xBF803820 */
  .extern OC5RSCLR         /* 0xBF803824 */
  .extern OC5RSSET         /* 0xBF803828 */
  .extern OC5RSINV         /* 0xBF80382C */
  .extern I2C1CON          /* 0xBF805000 */
  .extern I2C1ACON         /* 0xBF805000 */
  .extern I2C1CONCLR       /* 0xBF805004 */
  .extern I2C1ACONCLR      /* 0xBF805004 */
  .extern I2C1CONSET       /* 0xBF805008 */
  .extern I2C1ACONSET      /* 0xBF805008 */
  .extern I2C1CONINV       /* 0xBF80500C */
  .extern I2C1ACONINV      /* 0xBF80500C */
  .extern I2C1STAT         /* 0xBF805010 */
  .extern I2C1ASTAT        /* 0xBF805010 */
  .extern I2C1STATCLR      /* 0xBF805014 */
  .extern I2C1ASTATCLR     /* 0xBF805014 */
  .extern I2C1STATSET      /* 0xBF805018 */
  .extern I2C1ASTATSET     /* 0xBF805018 */
  .extern I2C1STATINV      /* 0xBF80501C */
  .extern I2C1ASTATINV     /* 0xBF80501C */
  .extern I2C1ADD          /* 0xBF805020 */
  .extern I2C1AADD         /* 0xBF805020 */
  .extern I2C1ADDCLR       /* 0xBF805024 */
  .extern I2C1AADDCLR      /* 0xBF805024 */
  .extern I2C1ADDSET       /* 0xBF805028 */
  .extern I2C1AADDSET      /* 0xBF805028 */
  .extern I2C1ADDINV       /* 0xBF80502C */
  .extern I2C1AADDINV      /* 0xBF80502C */
  .extern I2C1MSK          /* 0xBF805030 */
  .extern I2C1AMSK         /* 0xBF805030 */
  .extern I2C1MSKCLR       /* 0xBF805034 */
  .extern I2C1AMSKCLR      /* 0xBF805034 */
  .extern I2C1MSKSET       /* 0xBF805038 */
  .extern I2C1AMSKSET      /* 0xBF805038 */
  .extern I2C1MSKINV       /* 0xBF80503C */
  .extern I2C1AMSKINV      /* 0xBF80503C */
  .extern I2C1BRG          /* 0xBF805040 */
  .extern I2C1ABRG         /* 0xBF805040 */
  .extern I2C1BRGCLR       /* 0xBF805044 */
  .extern I2C1ABRGCLR      /* 0xBF805044 */
  .extern I2C1BRGSET       /* 0xBF805048 */
  .extern I2C1ABRGSET      /* 0xBF805048 */
  .extern I2C1BRGINV       /* 0xBF80504C */
  .extern I2C1ABRGINV      /* 0xBF80504C */
  .extern I2C1TRN          /* 0xBF805050 */
  .extern I2C1ATRN         /* 0xBF805050 */
  .extern I2C1TRNCLR       /* 0xBF805054 */
  .extern I2C1ATRNCLR      /* 0xBF805054 */
  .extern I2C1TRNSET       /* 0xBF805058 */
  .extern I2C1ATRNSET      /* 0xBF805058 */
  .extern I2C1TRNINV       /* 0xBF80505C */
  .extern I2C1ATRNINV      /* 0xBF80505C */
  .extern I2C1RCV          /* 0xBF805060 */
  .extern I2C1ARCV         /* 0xBF805060 */
  .extern I2C2CON          /* 0xBF805100 */
  .extern I2C2ACON         /* 0xBF805100 */
  .extern I2C2CONCLR       /* 0xBF805104 */
  .extern I2C2ACONCLR      /* 0xBF805104 */
  .extern I2C2CONSET       /* 0xBF805108 */
  .extern I2C2ACONSET      /* 0xBF805108 */
  .extern I2C2CONINV       /* 0xBF80510C */
  .extern I2C2ACONINV      /* 0xBF80510C */
  .extern I2C2STAT         /* 0xBF805110 */
  .extern I2C2ASTAT        /* 0xBF805110 */
  .extern I2C2STATCLR      /* 0xBF805114 */
  .extern I2C2ASTATCLR     /* 0xBF805114 */
  .extern I2C2STATSET      /* 0xBF805118 */
  .extern I2C2ASTATSET     /* 0xBF805118 */
  .extern I2C2STATINV      /* 0xBF80511C */
  .extern I2C2ASTATINV     /* 0xBF80511C */
  .extern I2C2ADD          /* 0xBF805120 */
  .extern I2C2AADD         /* 0xBF805120 */
  .extern I2C2ADDCLR       /* 0xBF805124 */
  .extern I2C2AADDCLR      /* 0xBF805124 */
  .extern I2C2ADDSET       /* 0xBF805128 */
  .extern I2C2AADDSET      /* 0xBF805128 */
  .extern I2C2ADDINV       /* 0xBF80512C */
  .extern I2C2AADDINV      /* 0xBF80512C */
  .extern I2C2MSK          /* 0xBF805130 */
  .extern I2C2AMSK         /* 0xBF805130 */
  .extern I2C2MSKCLR       /* 0xBF805134 */
  .extern I2C2AMSKCLR      /* 0xBF805134 */
  .extern I2C2MSKSET       /* 0xBF805138 */
  .extern I2C2AMSKSET      /* 0xBF805138 */
  .extern I2C2MSKINV       /* 0xBF80513C */
  .extern I2C2AMSKINV      /* 0xBF80513C */
  .extern I2C2BRG          /* 0xBF805140 */
  .extern I2C2ABRG         /* 0xBF805140 */
  .extern I2C2BRGCLR       /* 0xBF805144 */
  .extern I2C2ABRGCLR      /* 0xBF805144 */
  .extern I2C2BRGSET       /* 0xBF805148 */
  .extern I2C2ABRGSET      /* 0xBF805148 */
  .extern I2C2BRGINV       /* 0xBF80514C */
  .extern I2C2ABRGINV      /* 0xBF80514C */
  .extern I2C2TRN          /* 0xBF805150 */
  .extern I2C2ATRN         /* 0xBF805150 */
  .extern I2C2TRNCLR       /* 0xBF805154 */
  .extern I2C2ATRNCLR      /* 0xBF805154 */
  .extern I2C2TRNSET       /* 0xBF805158 */
  .extern I2C2ATRNSET      /* 0xBF805158 */
  .extern I2C2TRNINV       /* 0xBF80515C */
  .extern I2C2ATRNINV      /* 0xBF80515C */
  .extern I2C2RCV          /* 0xBF805160 */
  .extern I2C2ARCV         /* 0xBF805160 */
  .extern SPI1CON          /* 0xBF805800 */
  .extern SPI1CONCLR       /* 0xBF805804 */
  .extern SPI1CONSET       /* 0xBF805808 */
  .extern SPI1CONINV       /* 0xBF80580C */
  .extern SPI1STAT         /* 0xBF805810 */
  .extern SPI1STATCLR      /* 0xBF805814 */
  .extern SPI1STATSET      /* 0xBF805818 */
  .extern SPI1STATINV      /* 0xBF80581C */
  .extern SPI1BUF          /* 0xBF805820 */
  .extern SPI1BRG          /* 0xBF805830 */
  .extern SPI1BRGCLR       /* 0xBF805834 */
  .extern SPI1BRGSET       /* 0xBF805838 */
  .extern SPI1BRGINV       /* 0xBF80583C */
  .extern SPI1CON2         /* 0xBF805840 */
  .extern SPI1CON2CLR      /* 0xBF805844 */
  .extern SPI1CON2SET      /* 0xBF805848 */
  .extern SPI1CON2INV      /* 0xBF80584C */
  .extern SPI2CON          /* 0xBF805A00 */
  .extern SPI2CONCLR       /* 0xBF805A04 */
  .extern SPI2CONSET       /* 0xBF805A08 */
  .extern SPI2CONINV       /* 0xBF805A0C */
  .extern SPI2STAT         /* 0xBF805A10 */
  .extern SPI2STATCLR      /* 0xBF805A14 */
  .extern SPI2STATSET      /* 0xBF805A18 */
  .extern SPI2STATINV      /* 0xBF805A1C */
  .extern SPI2BUF          /* 0xBF805A20 */
  .extern SPI2BRG          /* 0xBF805A30 */
  .extern SPI2BRGCLR       /* 0xBF805A34 */
  .extern SPI2BRGSET       /* 0xBF805A38 */
  .extern SPI2BRGINV       /* 0xBF805A3C */
  .extern SPI2CON2         /* 0xBF805A40 */
  .extern SPI2CON2CLR      /* 0xBF805A44 */
  .extern SPI2CON2SET      /* 0xBF805A48 */
  .extern SPI2CON2INV      /* 0xBF805A4C */
  .extern U1MODE           /* 0xBF806000 */
  .extern U1AMODE          /* 0xBF806000 */
  .extern U1MODECLR        /* 0xBF806004 */
  .extern U1AMODECLR       /* 0xBF806004 */
  .extern U1MODESET        /* 0xBF806008 */
  .extern U1AMODESET       /* 0xBF806008 */
  .extern U1MODEINV        /* 0xBF80600C */
  .extern U1AMODEINV       /* 0xBF80600C */
  .extern U1STA            /* 0xBF806010 */
  .extern U1ASTA           /* 0xBF806010 */
  .extern U1STACLR         /* 0xBF806014 */
  .extern U1ASTACLR        /* 0xBF806014 */
  .extern U1STASET         /* 0xBF806018 */
  .extern U1ASTASET        /* 0xBF806018 */
  .extern U1STAINV         /* 0xBF80601C */
  .extern U1ASTAINV        /* 0xBF80601C */
  .extern U1TXREG          /* 0xBF806020 */
  .extern U1ATXREG         /* 0xBF806020 */
  .extern U1RXREG          /* 0xBF806030 */
  .extern U1ARXREG         /* 0xBF806030 */
  .extern U1BRG            /* 0xBF806040 */
  .extern U1ABRG           /* 0xBF806040 */
  .extern U1BRGCLR         /* 0xBF806044 */
  .extern U1ABRGCLR        /* 0xBF806044 */
  .extern U1BRGSET         /* 0xBF806048 */
  .extern U1ABRGSET        /* 0xBF806048 */
  .extern U1BRGINV         /* 0xBF80604C */
  .extern U1ABRGINV        /* 0xBF80604C */
  .extern U2MODE           /* 0xBF806200 */
  .extern U3AMODE          /* 0xBF806200 */
  .extern U2MODECLR        /* 0xBF806204 */
  .extern U3AMODECLR       /* 0xBF806204 */
  .extern U2MODESET        /* 0xBF806208 */
  .extern U3AMODESET       /* 0xBF806208 */
  .extern U2MODEINV        /* 0xBF80620C */
  .extern U3AMODEINV       /* 0xBF80620C */
  .extern U2STA            /* 0xBF806210 */
  .extern U3ASTA           /* 0xBF806210 */
  .extern U2STACLR         /* 0xBF806214 */
  .extern U3ASTACLR        /* 0xBF806214 */
  .extern U2STASET         /* 0xBF806218 */
  .extern U3ASTASET        /* 0xBF806218 */
  .extern U2STAINV         /* 0xBF80621C */
  .extern U3ASTAINV        /* 0xBF80621C */
  .extern U2TXREG          /* 0xBF806220 */
  .extern U3ATXREG         /* 0xBF806220 */
  .extern U2RXREG          /* 0xBF806230 */
  .extern U3ARXREG         /* 0xBF806230 */
  .extern U2BRG            /* 0xBF806240 */
  .extern U3ABRG           /* 0xBF806240 */
  .extern U2BRGCLR         /* 0xBF806244 */
  .extern U3ABRGCLR        /* 0xBF806244 */
  .extern U2BRGSET         /* 0xBF806248 */
  .extern U3ABRGSET        /* 0xBF806248 */
  .extern U2BRGINV         /* 0xBF80624C */
  .extern U3ABRGINV        /* 0xBF80624C */
  .extern PMCON            /* 0xBF807000 */
  .extern PMCONCLR         /* 0xBF807004 */
  .extern PMCONSET         /* 0xBF807008 */
  .extern PMCONINV         /* 0xBF80700C */
  .extern PMMODE           /* 0xBF807010 */
  .extern PMMODECLR        /* 0xBF807014 */
  .extern PMMODESET        /* 0xBF807018 */
  .extern PMMODEINV        /* 0xBF80701C */
  .extern PMADDR           /* 0xBF807020 */
  .extern PMADDRCLR        /* 0xBF807024 */
  .extern PMADDRSET        /* 0xBF807028 */
  .extern PMADDRINV        /* 0xBF80702C */
  .extern PMDOUT           /* 0xBF807030 */
  .extern PMDOUTCLR        /* 0xBF807034 */
  .extern PMDOUTSET        /* 0xBF807038 */
  .extern PMDOUTINV        /* 0xBF80703C */
  .extern PMDIN            /* 0xBF807040 */
  .extern PMDINCLR         /* 0xBF807044 */
  .extern PMDINSET         /* 0xBF807048 */
  .extern PMDININV         /* 0xBF80704C */
  .extern PMAEN            /* 0xBF807050 */
  .extern PMAENCLR         /* 0xBF807054 */
  .extern PMAENSET         /* 0xBF807058 */
  .extern PMAENINV         /* 0xBF80705C */
  .extern PMSTAT           /* 0xBF807060 */
  .extern PMSTATCLR        /* 0xBF807064 */
  .extern PMSTATSET        /* 0xBF807068 */
  .extern PMSTATINV        /* 0xBF80706C */
  .extern AD1CON1          /* 0xBF809000 */
  .extern AD1CON1CLR       /* 0xBF809004 */
  .extern AD1CON1SET       /* 0xBF809008 */
  .extern AD1CON1INV       /* 0xBF80900C */
  .extern AD1CON2          /* 0xBF809010 */
  .extern AD1CON2CLR       /* 0xBF809014 */
  .extern AD1CON2SET       /* 0xBF809018 */
  .extern AD1CON2INV       /* 0xBF80901C */
  .extern AD1CON3          /* 0xBF809020 */
  .extern AD1CON3CLR       /* 0xBF809024 */
  .extern AD1CON3SET       /* 0xBF809028 */
  .extern AD1CON3INV       /* 0xBF80902C */
  .extern AD1CHS           /* 0xBF809040 */
  .extern AD1CHSCLR        /* 0xBF809044 */
  .extern AD1CHSSET        /* 0xBF809048 */
  .extern AD1CHSINV        /* 0xBF80904C */
  .extern AD1CSSL          /* 0xBF809050 */
  .extern AD1CSSLCLR       /* 0xBF809054 */
  .extern AD1CSSLSET       /* 0xBF809058 */
  .extern AD1CSSLINV       /* 0xBF80905C */
  .extern ADC1BUF0         /* 0xBF809070 */
  .extern ADC1BUF1         /* 0xBF809080 */
  .extern ADC1BUF2         /* 0xBF809090 */
  .extern ADC1BUF3         /* 0xBF8090A0 */
  .extern ADC1BUF4         /* 0xBF8090B0 */
  .extern ADC1BUF5         /* 0xBF8090C0 */
  .extern ADC1BUF6         /* 0xBF8090D0 */
  .extern ADC1BUF7         /* 0xBF8090E0 */
  .extern ADC1BUF8         /* 0xBF8090F0 */
  .extern ADC1BUF9         /* 0xBF809100 */
  .extern ADC1BUFA         /* 0xBF809110 */
  .extern ADC1BUFB         /* 0xBF809120 */
  .extern ADC1BUFC         /* 0xBF809130 */
  .extern ADC1BUFD         /* 0xBF809140 */
  .extern ADC1BUFE         /* 0xBF809150 */
  .extern ADC1BUFF         /* 0xBF809160 */
  .extern CVRCON           /* 0xBF809800 */
  .extern CVRCONCLR        /* 0xBF809804 */
  .extern CVRCONSET        /* 0xBF809808 */
  .extern CVRCONINV        /* 0xBF80980C */
  .extern CM1CON           /* 0xBF80A000 */
  .extern CM1CONCLR        /* 0xBF80A004 */
  .extern CM1CONSET        /* 0xBF80A008 */
  .extern CM1CONINV        /* 0xBF80A00C */
  .extern CM2CON           /* 0xBF80A010 */
  .extern CM2CONCLR        /* 0xBF80A014 */
  .extern CM2CONSET        /* 0xBF80A018 */
  .extern CM2CONINV        /* 0xBF80A01C */
  .extern CM3CON           /* 0xBF80A020 */
  .extern CM3CONCLR        /* 0xBF80A024 */
  .extern CM3CONSET        /* 0xBF80A028 */
  .extern CM3CONINV        /* 0xBF80A02C */
  .extern CMSTAT           /* 0xBF80A060 */
  .extern CMSTATCLR        /* 0xBF80A064 */
  .extern CMSTATSET        /* 0xBF80A068 */
  .extern CMSTATINV        /* 0xBF80A06C */
  .extern CTMUCON          /* 0xBF80A200 */
  .extern CTMUCONCLR       /* 0xBF80A204 */
  .extern CTMUCONSET       /* 0xBF80A208 */
  .extern CTMUCONINV       /* 0xBF80A20C */
  .extern OSCCON           /* 0xBF80F000 */
  .extern OSCCONCLR        /* 0xBF80F004 */
  .extern OSCCONSET        /* 0xBF80F008 */
  .extern OSCCONINV        /* 0xBF80F00C */
  .extern OSCTUN           /* 0xBF80F010 */
  .extern OSCTUNCLR        /* 0xBF80F014 */
  .extern OSCTUNSET        /* 0xBF80F018 */
  .extern OSCTUNINV        /* 0xBF80F01C */
  .extern REFOCON          /* 0xBF80F020 */
  .extern REFOCONCLR       /* 0xBF80F024 */
  .extern REFOCONSET       /* 0xBF80F028 */
  .extern REFOCONINV       /* 0xBF80F02C */
  .extern REFOTRIM         /* 0xBF80F030 */
  .extern REFOTRIMCLR      /* 0xBF80F034 */
  .extern REFOTRIMSET      /* 0xBF80F038 */
  .extern REFOTRIMINV      /* 0xBF80F03C */
  .extern CFGCON           /* 0xBF80F200 */
  .extern DDPCON           /* 0xBF80F200 */
  .extern DEVID            /* 0xBF80F220 */
  .extern SYSKEY           /* 0xBF80F230 */
  .extern SYSKEYCLR        /* 0xBF80F234 */
  .extern SYSKEYSET        /* 0xBF80F238 */
  .extern SYSKEYINV        /* 0xBF80F23C */
  .extern PMD1             /* 0xBF80F240 */
  .extern PMD1CLR          /* 0xBF80F244 */
  .extern PMD1SET          /* 0xBF80F248 */
  .extern PMD1INV          /* 0xBF80F24C */
  .extern PMD2             /* 0xBF80F250 */
  .extern PMD2CLR          /* 0xBF80F254 */
  .extern PMD2SET          /* 0xBF80F258 */
  .extern PMD2INV          /* 0xBF80F25C */
  .extern PMD3             /* 0xBF80F260 */
  .extern PMD3CLR          /* 0xBF80F264 */
  .extern PMD3SET          /* 0xBF80F268 */
  .extern PMD3INV          /* 0xBF80F26C */
  .extern PMD4             /* 0xBF80F270 */
  .extern PMD4CLR          /* 0xBF80F274 */
  .extern PMD4SET          /* 0xBF80F278 */
  .extern PMD4INV          /* 0xBF80F27C */
  .extern PMD5             /* 0xBF80F280 */
  .extern PMD5CLR          /* 0xBF80F284 */
  .extern PMD5SET          /* 0xBF80F288 */
  .extern PMD5INV          /* 0xBF80F28C */
  .extern PMD6             /* 0xBF80F290 */
  .extern PMD6CLR          /* 0xBF80F294 */
  .extern PMD6SET          /* 0xBF80F298 */
  .extern PMD6INV          /* 0xBF80F29C */
  .extern NVMCON           /* 0xBF80F400 */
  .extern NVMCONCLR        /* 0xBF80F404 */
  .extern NVMCONSET        /* 0xBF80F408 */
  .extern NVMCONINV        /* 0xBF80F40C */
  .extern NVMKEY           /* 0xBF80F410 */
  .extern NVMADDR          /* 0xBF80F420 */
  .extern NVMADDRCLR       /* 0xBF80F424 */
  .extern NVMADDRSET       /* 0xBF80F428 */
  .extern NVMADDRINV       /* 0xBF80F42C */
  .extern NVMDATA          /* 0xBF80F430 */
  .extern NVMSRCADDR       /* 0xBF80F440 */
  .extern RCON             /* 0xBF80F600 */
  .extern RCONCLR          /* 0xBF80F604 */
  .extern RCONSET          /* 0xBF80F608 */
  .extern RCONINV          /* 0xBF80F60C */
  .extern RSWRST           /* 0xBF80F610 */
  .extern RSWRSTCLR        /* 0xBF80F614 */
  .extern RSWRSTSET        /* 0xBF80F618 */
  .extern RSWRSTINV        /* 0xBF80F61C */
  .extern INT1R            /* 0xBF80FA04 */
  .extern INT2R            /* 0xBF80FA08 */
  .extern INT3R            /* 0xBF80FA0C */
  .extern INT4R            /* 0xBF80FA10 */
  .extern T2CKR            /* 0xBF80FA18 */
  .extern T3CKR            /* 0xBF80FA1C */
  .extern T4CKR            /* 0xBF80FA20 */
  .extern T5CKR            /* 0xBF80FA24 */
  .extern IC1R             /* 0xBF80FA28 */
  .extern IC2R             /* 0xBF80FA2C */
  .extern IC3R             /* 0xBF80FA30 */
  .extern IC4R             /* 0xBF80FA34 */
  .extern IC5R             /* 0xBF80FA38 */
  .extern OCFAR            /* 0xBF80FA48 */
  .extern OCFBR            /* 0xBF80FA4C */
  .extern U1RXR            /* 0xBF80FA50 */
  .extern U1CTSR           /* 0xBF80FA54 */
  .extern U2RXR            /* 0xBF80FA58 */
  .extern U2CTSR           /* 0xBF80FA5C */
  .extern SDI1R            /* 0xBF80FA84 */
  .extern SS1R             /* 0xBF80FA88 */
  .extern SDI2R            /* 0xBF80FA90 */
  .extern SS2R             /* 0xBF80FA94 */
  .extern REFCLKIR         /* 0xBF80FAB8 */
  .extern RPA0R            /* 0xBF80FB00 */
  .extern RPA1R            /* 0xBF80FB04 */
  .extern RPA2R            /* 0xBF80FB08 */
  .extern RPA3R            /* 0xBF80FB0C */
  .extern RPA4R            /* 0xBF80FB10 */
  .extern RPA8R            /* 0xBF80FB20 */
  .extern RPA9R            /* 0xBF80FB24 */
  .extern RPB0R            /* 0xBF80FB2C */
  .extern RPB1R            /* 0xBF80FB30 */
  .extern RPB2R            /* 0xBF80FB34 */
  .extern RPB3R            /* 0xBF80FB38 */
  .extern RPB4R            /* 0xBF80FB3C */
  .extern RPB5R            /* 0xBF80FB40 */
  .extern RPB7R            /* 0xBF80FB48 */
  .extern RPB8R            /* 0xBF80FB4C */
  .extern RPB9R            /* 0xBF80FB50 */
  .extern RPB10R           /* 0xBF80FB54 */
  .extern RPB11R           /* 0xBF80FB58 */
  .extern RPB13R           /* 0xBF80FB60 */
  .extern RPB14R           /* 0xBF80FB64 */
  .extern RPB15R           /* 0xBF80FB68 */
  .extern RPC0R            /* 0xBF80FB6C */
  .extern RPC1R            /* 0xBF80FB70 */
  .extern RPC2R            /* 0xBF80FB74 */
  .extern RPC3R            /* 0xBF80FB78 */
  .extern RPC4R            /* 0xBF80FB7C */
  .extern RPC5R            /* 0xBF80FB80 */
  .extern RPC6R            /* 0xBF80FB84 */
  .extern RPC7R            /* 0xBF80FB88 */
  .extern RPC8R            /* 0xBF80FB8C */
  .extern RPC9R            /* 0xBF80FB90 */
  .extern INTCON           /* 0xBF881000 */
  .extern INTCONCLR        /* 0xBF881004 */
  .extern INTCONSET        /* 0xBF881008 */
  .extern INTCONINV        /* 0xBF88100C */
  .extern INTSTAT          /* 0xBF881010 */
  .extern IPTMR            /* 0xBF881020 */
  .extern IPTMRCLR         /* 0xBF881024 */
  .extern IPTMRSET         /* 0xBF881028 */
  .extern IPTMRINV         /* 0xBF88102C */
  .extern IFS0             /* 0xBF881030 */
  .extern IFS0CLR          /* 0xBF881034 */
  .extern IFS0SET          /* 0xBF881038 */
  .extern IFS0INV          /* 0xBF88103C */
  .extern IFS1             /* 0xBF881040 */
  .extern IFS1CLR          /* 0xBF881044 */
  .extern IFS1SET          /* 0xBF881048 */
  .extern IFS1INV          /* 0xBF88104C */
  .extern IEC0             /* 0xBF881060 */
  .extern IEC0CLR          /* 0xBF881064 */
  .extern IEC0SET          /* 0xBF881068 */
  .extern IEC0INV          /* 0xBF88106C */
  .extern IEC1             /* 0xBF881070 */
  .extern IEC1CLR          /* 0xBF881074 */
  .extern IEC1SET          /* 0xBF881078 */
  .extern IEC1INV          /* 0xBF88107C */
  .extern IPC0             /* 0xBF881090 */
  .extern IPC0CLR          /* 0xBF881094 */
  .extern IPC0SET          /* 0xBF881098 */
  .extern IPC0INV          /* 0xBF88109C */
  .extern IPC1             /* 0xBF8810A0 */
  .extern IPC1CLR          /* 0xBF8810A4 */
  .extern IPC1SET          /* 0xBF8810A8 */
  .extern IPC1INV          /* 0xBF8810AC */
  .extern IPC2             /* 0xBF8810B0 */
  .extern IPC2CLR          /* 0xBF8810B4 */
  .extern IPC2SET          /* 0xBF8810B8 */
  .extern IPC2INV          /* 0xBF8810BC */
  .extern IPC3             /* 0xBF8810C0 */
  .extern IPC3CLR          /* 0xBF8810C4 */
  .extern IPC3SET          /* 0xBF8810C8 */
  .extern IPC3INV          /* 0xBF8810CC */
  .extern IPC4             /* 0xBF8810D0 */
  .extern IPC4CLR          /* 0xBF8810D4 */
  .extern IPC4SET          /* 0xBF8810D8 */
  .extern IPC4INV          /* 0xBF8810DC */
  .extern IPC5             /* 0xBF8810E0 */
  .extern IPC5CLR          /* 0xBF8810E4 */
  .extern IPC5SET          /* 0xBF8810E8 */
  .extern IPC5INV          /* 0xBF8810EC */
  .extern IPC6             /* 0xBF8810F0 */
  .extern IPC6CLR          /* 0xBF8810F4 */
  .extern IPC6SET          /* 0xBF8810F8 */
  .extern IPC6INV          /* 0xBF8810FC */
  .extern IPC7             /* 0xBF881100 */
  .extern IPC7CLR          /* 0xBF881104 */
  .extern IPC7SET          /* 0xBF881108 */
  .extern IPC7INV          /* 0xBF88110C */
  .extern IPC8             /* 0xBF881110 */
  .extern IPC8CLR          /* 0xBF881114 */
  .extern IPC8SET          /* 0xBF881118 */
  .extern IPC8INV          /* 0xBF88111C */
  .extern IPC9             /* 0xBF881120 */
  .extern IPC9CLR          /* 0xBF881124 */
  .extern IPC9SET          /* 0xBF881128 */
  .extern IPC9INV          /* 0xBF88112C */
  .extern IPC10            /* 0xBF881130 */
  .extern IPC10CLR         /* 0xBF881134 */
  .extern IPC10SET         /* 0xBF881138 */
  .extern IPC10INV         /* 0xBF88113C */
  .extern BMXCON           /* 0xBF882000 */
  .extern BMXCONCLR        /* 0xBF882004 */
  .extern BMXCONSET        /* 0xBF882008 */
  .extern BMXCONINV        /* 0xBF88200C */
  .extern BMXDKPBA         /* 0xBF882010 */
  .extern BMXDKPBACLR      /* 0xBF882014 */
  .extern BMXDKPBASET      /* 0xBF882018 */
  .extern BMXDKPBAINV      /* 0xBF88201C */
  .extern BMXDUDBA         /* 0xBF882020 */
  .extern BMXDUDBACLR      /* 0xBF882024 */
  .extern BMXDUDBASET      /* 0xBF882028 */
  .extern BMXDUDBAINV      /* 0xBF88202C */
  .extern BMXDUPBA         /* 0xBF882030 */
  .extern BMXDUPBACLR      /* 0xBF882034 */
  .extern BMXDUPBASET      /* 0xBF882038 */
  .extern BMXDUPBAINV      /* 0xBF88203C */
  .extern BMXDRMSZ         /* 0xBF882040 */
  .extern BMXPUPBA         /* 0xBF882050 */
  .extern BMXPUPBACLR      /* 0xBF882054 */
  .extern BMXPUPBASET      /* 0xBF882058 */
  .extern BMXPUPBAINV      /* 0xBF88205C */
  .extern BMXPFMSZ         /* 0xBF882060 */
  .extern BMXBOOTSZ        /* 0xBF882070 */
  .extern DMACON           /* 0xBF883000 */
  .extern DMACONCLR        /* 0xBF883004 */
  .extern DMACONSET        /* 0xBF883008 */
  .extern DMACONINV        /* 0xBF88300C */
  .extern DMASTAT          /* 0xBF883010 */
  .extern DMASTATCLR       /* 0xBF883014 */
  .extern DMASTATSET       /* 0xBF883018 */
  .extern DMASTATINV       /* 0xBF88301C */
  .extern DMAADDR          /* 0xBF883020 */
  .extern DMAADDRCLR       /* 0xBF883024 */
  .extern DMAADDRSET       /* 0xBF883028 */
  .extern DMAADDRINV       /* 0xBF88302C */
  .extern DCRCCON          /* 0xBF883030 */
  .extern DCRCCONCLR       /* 0xBF883034 */
  .extern DCRCCONSET       /* 0xBF883038 */
  .extern DCRCCONINV       /* 0xBF88303C */
  .extern DCRCDATA         /* 0xBF883040 */
  .extern DCRCDATACLR      /* 0xBF883044 */
  .extern DCRCDATASET      /* 0xBF883048 */
  .extern DCRCDATAINV      /* 0xBF88304C */
  .extern DCRCXOR          /* 0xBF883050 */
  .extern DCRCXORCLR       /* 0xBF883054 */
  .extern DCRCXORSET       /* 0xBF883058 */
  .extern DCRCXORINV       /* 0xBF88305C */
  .extern DCH0CON          /* 0xBF883060 */
  .extern DCH0CONCLR       /* 0xBF883064 */
  .extern DCH0CONSET       /* 0xBF883068 */
  .extern DCH0CONINV       /* 0xBF88306C */
  .extern DCH0ECON         /* 0xBF883070 */
  .extern DCH0ECONCLR      /* 0xBF883074 */
  .extern DCH0ECONSET      /* 0xBF883078 */
  .extern DCH0ECONINV      /* 0xBF88307C */
  .extern DCH0INT          /* 0xBF883080 */
  .extern DCH0INTCLR       /* 0xBF883084 */
  .extern DCH0INTSET       /* 0xBF883088 */
  .extern DCH0INTINV       /* 0xBF88308C */
  .extern DCH0SSA          /* 0xBF883090 */
  .extern DCH0SSACLR       /* 0xBF883094 */
  .extern DCH0SSASET       /* 0xBF883098 */
  .extern DCH0SSAINV       /* 0xBF88309C */
  .extern DCH0DSA          /* 0xBF8830A0 */
  .extern DCH0DSACLR       /* 0xBF8830A4 */
  .extern DCH0DSASET       /* 0xBF8830A8 */
  .extern DCH0DSAINV       /* 0xBF8830AC */
  .extern DCH0SSIZ         /* 0xBF8830B0 */
  .extern DCH0SSIZCLR      /* 0xBF8830B4 */
  .extern DCH0SSIZSET      /* 0xBF8830B8 */
  .extern DCH0SSIZINV      /* 0xBF8830BC */
  .extern DCH0DSIZ         /* 0xBF8830C0 */
  .extern DCH0DSIZCLR      /* 0xBF8830C4 */
  .extern DCH0DSIZSET      /* 0xBF8830C8 */
  .extern DCH0DSIZINV      /* 0xBF8830CC */
  .extern DCH0SPTR         /* 0xBF8830D0 */
  .extern DCH0SPTRCLR      /* 0xBF8830D4 */
  .extern DCH0SPTRSET      /* 0xBF8830D8 */
  .extern DCH0SPTRINV      /* 0xBF8830DC */
  .extern DCH0DPTR         /* 0xBF8830E0 */
  .extern DCH0DPTRCLR      /* 0xBF8830E4 */
  .extern DCH0DPTRSET      /* 0xBF8830E8 */
  .extern DCH0DPTRINV      /* 0xBF8830EC */
  .extern DCH0CSIZ         /* 0xBF8830F0 */
  .extern DCH0CSIZCLR      /* 0xBF8830F4 */
  .extern DCH0CSIZSET      /* 0xBF8830F8 */
  .extern DCH0CSIZINV      /* 0xBF8830FC */
  .extern DCH0CPTR         /* 0xBF883100 */
  .extern DCH0CPTRCLR      /* 0xBF883104 */
  .extern DCH0CPTRSET      /* 0xBF883108 */
  .extern DCH0CPTRINV      /* 0xBF88310C */
  .extern DCH0DAT          /* 0xBF883110 */
  .extern DCH0DATCLR       /* 0xBF883114 */
  .extern DCH0DATSET       /* 0xBF883118 */
  .extern DCH0DATINV       /* 0xBF88311C */
  .extern DCH1CON          /* 0xBF883120 */
  .extern DCH1CONCLR       /* 0xBF883124 */
  .extern DCH1CONSET       /* 0xBF883128 */
  .extern DCH1CONINV       /* 0xBF88312C */
  .extern DCH1ECON         /* 0xBF883130 */
  .extern DCH1ECONCLR      /* 0xBF883134 */
  .extern DCH1ECONSET      /* 0xBF883138 */
  .extern DCH1ECONINV      /* 0xBF88313C */
  .extern DCH1INT          /* 0xBF883140 */
  .extern DCH1INTCLR       /* 0xBF883144 */
  .extern DCH1INTSET       /* 0xBF883148 */
  .extern DCH1INTINV       /* 0xBF88314C */
  .extern DCH1SSA          /* 0xBF883150 */
  .extern DCH1SSACLR       /* 0xBF883154 */
  .extern DCH1SSASET       /* 0xBF883158 */
  .extern DCH1SSAINV       /* 0xBF88315C */
  .extern DCH1DSA          /* 0xBF883160 */
  .extern DCH1DSACLR       /* 0xBF883164 */
  .extern DCH1DSASET       /* 0xBF883168 */
  .extern DCH1DSAINV       /* 0xBF88316C */
  .extern DCH1SSIZ         /* 0xBF883170 */
  .extern DCH1SSIZCLR      /* 0xBF883174 */
  .extern DCH1SSIZSET      /* 0xBF883178 */
  .extern DCH1SSIZINV      /* 0xBF88317C */
  .extern DCH1DSIZ         /* 0xBF883180 */
  .extern DCH1DSIZCLR      /* 0xBF883184 */
  .extern DCH1DSIZSET      /* 0xBF883188 */
  .extern DCH1DSIZINV      /* 0xBF88318C */
  .extern DCH1SPTR         /* 0xBF883190 */
  .extern DCH1SPTRCLR      /* 0xBF883194 */
  .extern DCH1SPTRSET      /* 0xBF883198 */
  .extern DCH1SPTRINV      /* 0xBF88319C */
  .extern DCH1DPTR         /* 0xBF8831A0 */
  .extern DCH1DPTRCLR      /* 0xBF8831A4 */
  .extern DCH1DPTRSET      /* 0xBF8831A8 */
  .extern DCH1DPTRINV      /* 0xBF8831AC */
  .extern DCH1CSIZ         /* 0xBF8831B0 */
  .extern DCH1CSIZCLR      /* 0xBF8831B4 */
  .extern DCH1CSIZSET      /* 0xBF8831B8 */
  .extern DCH1CSIZINV      /* 0xBF8831BC */
  .extern DCH1CPTR         /* 0xBF8831C0 */
  .extern DCH1CPTRCLR      /* 0xBF8831C4 */
  .extern DCH1CPTRSET      /* 0xBF8831C8 */
  .extern DCH1CPTRINV      /* 0xBF8831CC */
  .extern DCH1DAT          /* 0xBF8831D0 */
  .extern DCH1DATCLR       /* 0xBF8831D4 */
  .extern DCH1DATSET       /* 0xBF8831D8 */
  .extern DCH1DATINV       /* 0xBF8831DC */
  .extern DCH2CON          /* 0xBF8831E0 */
  .extern DCH2CONCLR       /* 0xBF8831E4 */
  .extern DCH2CONSET       /* 0xBF8831E8 */
  .extern DCH2CONINV       /* 0xBF8831EC */
  .extern DCH2ECON         /* 0xBF8831F0 */
  .extern DCH2ECONCLR      /* 0xBF8831F4 */
  .extern DCH2ECONSET      /* 0xBF8831F8 */
  .extern DCH2ECONINV      /* 0xBF8831FC */
  .extern DCH2INT          /* 0xBF883200 */
  .extern DCH2INTCLR       /* 0xBF883204 */
  .extern DCH2INTSET       /* 0xBF883208 */
  .extern DCH2INTINV       /* 0xBF88320C */
  .extern DCH2SSA          /* 0xBF883210 */
  .extern DCH2SSACLR       /* 0xBF883214 */
  .extern DCH2SSASET       /* 0xBF883218 */
  .extern DCH2SSAINV       /* 0xBF88321C */
  .extern DCH2DSA          /* 0xBF883220 */
  .extern DCH2DSACLR       /* 0xBF883224 */
  .extern DCH2DSASET       /* 0xBF883228 */
  .extern DCH2DSAINV       /* 0xBF88322C */
  .extern DCH2SSIZ         /* 0xBF883230 */
  .extern DCH2SSIZCLR      /* 0xBF883234 */
  .extern DCH2SSIZSET      /* 0xBF883238 */
  .extern DCH2SSIZINV      /* 0xBF88323C */
  .extern DCH2DSIZ         /* 0xBF883240 */
  .extern DCH2DSIZCLR      /* 0xBF883244 */
  .extern DCH2DSIZSET      /* 0xBF883248 */
  .extern DCH2DSIZINV      /* 0xBF88324C */
  .extern DCH2SPTR         /* 0xBF883250 */
  .extern DCH2SPTRCLR      /* 0xBF883254 */
  .extern DCH2SPTRSET      /* 0xBF883258 */
  .extern DCH2SPTRINV      /* 0xBF88325C */
  .extern DCH2DPTR         /* 0xBF883260 */
  .extern DCH2DPTRCLR      /* 0xBF883264 */
  .extern DCH2DPTRSET      /* 0xBF883268 */
  .extern DCH2DPTRINV      /* 0xBF88326C */
  .extern DCH2CSIZ         /* 0xBF883270 */
  .extern DCH2CSIZCLR      /* 0xBF883274 */
  .extern DCH2CSIZSET      /* 0xBF883278 */
  .extern DCH2CSIZINV      /* 0xBF88327C */
  .extern DCH2CPTR         /* 0xBF883280 */
  .extern DCH2CPTRCLR      /* 0xBF883284 */
  .extern DCH2CPTRSET      /* 0xBF883288 */
  .extern DCH2CPTRINV      /* 0xBF88328C */
  .extern DCH2DAT          /* 0xBF883290 */
  .extern DCH2DATCLR       /* 0xBF883294 */
  .extern DCH2DATSET       /* 0xBF883298 */
  .extern DCH2DATINV       /* 0xBF88329C */
  .extern DCH3CON          /* 0xBF8832A0 */
  .extern DCH3CONCLR       /* 0xBF8832A4 */
  .extern DCH3CONSET       /* 0xBF8832A8 */
  .extern DCH3CONINV       /* 0xBF8832AC */
  .extern DCH3ECON         /* 0xBF8832B0 */
  .extern DCH3ECONCLR      /* 0xBF8832B4 */
  .extern DCH3ECONSET      /* 0xBF8832B8 */
  .extern DCH3ECONINV      /* 0xBF8832BC */
  .extern DCH3INT          /* 0xBF8832C0 */
  .extern DCH3INTCLR       /* 0xBF8832C4 */
  .extern DCH3INTSET       /* 0xBF8832C8 */
  .extern DCH3INTINV       /* 0xBF8832CC */
  .extern DCH3SSA          /* 0xBF8832D0 */
  .extern DCH3SSACLR       /* 0xBF8832D4 */
  .extern DCH3SSASET       /* 0xBF8832D8 */
  .extern DCH3SSAINV       /* 0xBF8832DC */
  .extern DCH3DSA          /* 0xBF8832E0 */
  .extern DCH3DSACLR       /* 0xBF8832E4 */
  .extern DCH3DSASET       /* 0xBF8832E8 */
  .extern DCH3DSAINV       /* 0xBF8832EC */
  .extern DCH3SSIZ         /* 0xBF8832F0 */
  .extern DCH3SSIZCLR      /* 0xBF8832F4 */
  .extern DCH3SSIZSET      /* 0xBF8832F8 */
  .extern DCH3SSIZINV      /* 0xBF8832FC */
  .extern DCH3DSIZ         /* 0xBF883300 */
  .extern DCH3DSIZCLR      /* 0xBF883304 */
  .extern DCH3DSIZSET      /* 0xBF883308 */
  .extern DCH3DSIZINV      /* 0xBF88330C */
  .extern DCH3SPTR         /* 0xBF883310 */
  .extern DCH3SPTRCLR      /* 0xBF883314 */
  .extern DCH3SPTRSET      /* 0xBF883318 */
  .extern DCH3SPTRINV      /* 0xBF88331C */
  .extern DCH3DPTR         /* 0xBF883320 */
  .extern DCH3DPTRCLR      /* 0xBF883324 */
  .extern DCH3DPTRSET      /* 0xBF883328 */
  .extern DCH3DPTRINV      /* 0xBF88332C */
  .extern DCH3CSIZ         /* 0xBF883330 */
  .extern DCH3CSIZCLR      /* 0xBF883334 */
  .extern DCH3CSIZSET      /* 0xBF883338 */
  .extern DCH3CSIZINV      /* 0xBF88333C */
  .extern DCH3CPTR         /* 0xBF883340 */
  .extern DCH3CPTRCLR      /* 0xBF883344 */
  .extern DCH3CPTRSET      /* 0xBF883348 */
  .extern DCH3CPTRINV      /* 0xBF88334C */
  .extern DCH3DAT          /* 0xBF883350 */
  .extern DCH3DATCLR       /* 0xBF883354 */
  .extern DCH3DATSET       /* 0xBF883358 */
  .extern DCH3DATINV       /* 0xBF88335C */
  .extern U1OTGIR          /* 0xBF885040 */
  .extern U1OTGIRCLR       /* 0xBF885044 */
  .extern U1OTGIE          /* 0xBF885050 */
  .extern U1OTGIECLR       /* 0xBF885054 */
  .extern U1OTGIESET       /* 0xBF885058 */
  .extern U1OTGIEINV       /* 0xBF88505C */
  .extern U1OTGSTAT        /* 0xBF885060 */
  .extern U1OTGCON         /* 0xBF885070 */
  .extern U1OTGCONCLR      /* 0xBF885074 */
  .extern U1OTGCONSET      /* 0xBF885078 */
  .extern U1OTGCONINV      /* 0xBF88507C */
  .extern U1PWRC           /* 0xBF885080 */
  .extern U1PWRCCLR        /* 0xBF885084 */
  .extern U1PWRCSET        /* 0xBF885088 */
  .extern U1PWRCINV        /* 0xBF88508C */
  .extern U1IR             /* 0xBF885200 */
  .extern U1IRCLR          /* 0xBF885204 */
  .extern U1IE             /* 0xBF885210 */
  .extern U1IECLR          /* 0xBF885214 */
  .extern U1IESET          /* 0xBF885218 */
  .extern U1IEINV          /* 0xBF88521C */
  .extern U1EIR            /* 0xBF885220 */
  .extern U1EIRCLR         /* 0xBF885224 */
  .extern U1EIE            /* 0xBF885230 */
  .extern U1EIECLR         /* 0xBF885234 */
  .extern U1EIESET         /* 0xBF885238 */
  .extern U1EIEINV         /* 0xBF88523C */
  .extern U1STAT           /* 0xBF885240 */
  .extern U1CON            /* 0xBF885250 */
  .extern U1CONCLR         /* 0xBF885254 */
  .extern U1CONSET         /* 0xBF885258 */
  .extern U1CONINV         /* 0xBF88525C */
  .extern U1ADDR           /* 0xBF885260 */
  .extern U1ADDRCLR        /* 0xBF885264 */
  .extern U1ADDRSET        /* 0xBF885268 */
  .extern U1ADDRINV        /* 0xBF88526C */
  .extern U1BDTP1          /* 0xBF885270 */
  .extern U1BDTP1CLR       /* 0xBF885274 */
  .extern U1BDTP1SET       /* 0xBF885278 */
  .extern U1BDTP1INV       /* 0xBF88527C */
  .extern U1FRML           /* 0xBF885280 */
  .extern U1FRMH           /* 0xBF885290 */
  .extern U1TOK            /* 0xBF8852A0 */
  .extern U1TOKCLR         /* 0xBF8852A4 */
  .extern U1TOKSET         /* 0xBF8852A8 */
  .extern U1TOKINV         /* 0xBF8852AC */
  .extern U1SOF            /* 0xBF8852B0 */
  .extern U1SOFCLR         /* 0xBF8852B4 */
  .extern U1SOFSET         /* 0xBF8852B8 */
  .extern U1SOFINV         /* 0xBF8852BC */
  .extern U1BDTP2          /* 0xBF8852C0 */
  .extern U1BDTP2CLR       /* 0xBF8852C4 */
  .extern U1BDTP2SET       /* 0xBF8852C8 */
  .extern U1BDTP2INV       /* 0xBF8852CC */
  .extern U1BDTP3          /* 0xBF8852D0 */
  .extern U1BDTP3CLR       /* 0xBF8852D4 */
  .extern U1BDTP3SET       /* 0xBF8852D8 */
  .extern U1BDTP3INV       /* 0xBF8852DC */
  .extern U1CNFG1          /* 0xBF8852E0 */
  .extern U1CNFG1CLR       /* 0xBF8852E4 */
  .extern U1CNFG1SET       /* 0xBF8852E8 */
  .extern U1CNFG1INV       /* 0xBF8852EC */
  .extern U1EP0            /* 0xBF885300 */
  .extern U1EP0CLR         /* 0xBF885304 */
  .extern U1EP0SET         /* 0xBF885308 */
  .extern U1EP0INV         /* 0xBF88530C */
  .extern U1EP1            /* 0xBF885310 */
  .extern U1EP1CLR         /* 0xBF885314 */
  .extern U1EP1SET         /* 0xBF885318 */
  .extern U1EP1INV         /* 0xBF88531C */
  .extern U1EP2            /* 0xBF885320 */
  .extern U1EP2CLR         /* 0xBF885324 */
  .extern U1EP2SET         /* 0xBF885328 */
  .extern U1EP2INV         /* 0xBF88532C */
  .extern U1EP3            /* 0xBF885330 */
  .extern U1EP3CLR         /* 0xBF885334 */
  .extern U1EP3SET         /* 0xBF885338 */
  .extern U1EP3INV         /* 0xBF88533C */
  .extern U1EP4            /* 0xBF885340 */
  .extern U1EP4CLR         /* 0xBF885344 */
  .extern U1EP4SET         /* 0xBF885348 */
  .extern U1EP4INV         /* 0xBF88534C */
  .extern U1EP5            /* 0xBF885350 */
  .extern U1EP5CLR         /* 0xBF885354 */
  .extern U1EP5SET         /* 0xBF885358 */
  .extern U1EP5INV         /* 0xBF88535C */
  .extern U1EP6            /* 0xBF885360 */
  .extern U1EP6CLR         /* 0xBF885364 */
  .extern U1EP6SET         /* 0xBF885368 */
  .extern U1EP6INV         /* 0xBF88536C */
  .extern U1EP7            /* 0xBF885370 */
  .extern U1EP7CLR         /* 0xBF885374 */
  .extern U1EP7SET         /* 0xBF885378 */
  .extern U1EP7INV         /* 0xBF88537C */
  .extern U1EP8            /* 0xBF885380 */
  .extern U1EP8CLR         /* 0xBF885384 */
  .extern U1EP8SET         /* 0xBF885388 */
  .extern U1EP8INV         /* 0xBF88538C */
  .extern U1EP9            /* 0xBF885390 */
  .extern U1EP9CLR         /* 0xBF885394 */
  .extern U1EP9SET         /* 0xBF885398 */
  .extern U1EP9INV         /* 0xBF88539C */
  .extern U1EP10           /* 0xBF8853A0 */
  .extern U1EP10CLR        /* 0xBF8853A4 */
  .extern U1EP10SET        /* 0xBF8853A8 */
  .extern U1EP10INV        /* 0xBF8853AC */
  .extern U1EP11           /* 0xBF8853B0 */
  .extern U1EP11CLR        /* 0xBF8853B4 */
  .extern U1EP11SET        /* 0xBF8853B8 */
  .extern U1EP11INV        /* 0xBF8853BC */
  .extern U1EP12           /* 0xBF8853C0 */
  .extern U1EP12CLR        /* 0xBF8853C4 */
  .extern U1EP12SET        /* 0xBF8853C8 */
  .extern U1EP12INV        /* 0xBF8853CC */
  .extern U1EP13           /* 0xBF8853D0 */
  .extern U1EP13CLR        /* 0xBF8853D4 */
  .extern U1EP13SET        /* 0xBF8853D8 */
  .extern U1EP13INV        /* 0xBF8853DC */
  .extern U1EP14           /* 0xBF8853E0 */
  .extern U1EP14CLR        /* 0xBF8853E4 */
  .extern U1EP14SET        /* 0xBF8853E8 */
  .extern U1EP14INV        /* 0xBF8853EC */
  .extern U1EP15           /* 0xBF8853F0 */
  .extern U1EP15CLR        /* 0xBF8853F4 */
  .extern U1EP15SET        /* 0xBF8853F8 */
  .extern U1EP15INV        /* 0xBF8853FC */
  .extern ANSELA           /* 0xBF886000 */
  .extern ANSELACLR        /* 0xBF886004 */
  .extern ANSELASET        /* 0xBF886008 */
  .extern ANSELAINV        /* 0xBF88600C */
  .extern TRISA            /* 0xBF886010 */
  .extern TRISACLR         /* 0xBF886014 */
  .extern TRISASET         /* 0xBF886018 */
  .extern TRISAINV         /* 0xBF88601C */
  .extern PORTA            /* 0xBF886020 */
  .extern PORTACLR         /* 0xBF886024 */
  .extern PORTASET         /* 0xBF886028 */
  .extern PORTAINV         /* 0xBF88602C */
  .extern LATA             /* 0xBF886030 */
  .extern LATACLR          /* 0xBF886034 */
  .extern LATASET          /* 0xBF886038 */
  .extern LATAINV          /* 0xBF88603C */
  .extern ODCA             /* 0xBF886040 */
  .extern ODCACLR          /* 0xBF886044 */
  .extern ODCASET          /* 0xBF886048 */
  .extern ODCAINV          /* 0xBF88604C */
  .extern CNPUA            /* 0xBF886050 */
  .extern CNPUACLR         /* 0xBF886054 */
  .extern CNPUASET         /* 0xBF886058 */
  .extern CNPUAINV         /* 0xBF88605C */
  .extern CNPDA            /* 0xBF886060 */
  .extern CNPDACLR         /* 0xBF886064 */
  .extern CNPDASET         /* 0xBF886068 */
  .extern CNPDAINV         /* 0xBF88606C */
  .extern CNCONA           /* 0xBF886070 */
  .extern CNCONACLR        /* 0xBF886074 */
  .extern CNCONASET        /* 0xBF886078 */
  .extern CNCONAINV        /* 0xBF88607C */
  .extern CNENA            /* 0xBF886080 */
  .extern CNENACLR         /* 0xBF886084 */
  .extern CNENASET         /* 0xBF886088 */
  .extern CNENAINV         /* 0xBF88608C */
  .extern CNSTATA          /* 0xBF886090 */
  .extern CNSTATACLR       /* 0xBF886094 */
  .extern CNSTATASET       /* 0xBF886098 */
  .extern CNSTATAINV       /* 0xBF88609C */
  .extern ANSELB           /* 0xBF886100 */
  .extern ANSELBCLR        /* 0xBF886104 */
  .extern ANSELBSET        /* 0xBF886108 */
  .extern ANSELBINV        /* 0xBF88610C */
  .extern TRISB            /* 0xBF886110 */
  .extern TRISBCLR         /* 0xBF886114 */
  .extern TRISBSET         /* 0xBF886118 */
  .extern TRISBINV         /* 0xBF88611C */
  .extern PORTB            /* 0xBF886120 */
  .extern PORTBCLR         /* 0xBF886124 */
  .extern PORTBSET         /* 0xBF886128 */
  .extern PORTBINV         /* 0xBF88612C */
  .extern LATB             /* 0xBF886130 */
  .extern LATBCLR          /* 0xBF886134 */
  .extern LATBSET          /* 0xBF886138 */
  .extern LATBINV          /* 0xBF88613C */
  .extern ODCB             /* 0xBF886140 */
  .extern ODCBCLR          /* 0xBF886144 */
  .extern ODCBSET          /* 0xBF886148 */
  .extern ODCBINV          /* 0xBF88614C */
  .extern CNPUB            /* 0xBF886150 */
  .extern CNPUBCLR         /* 0xBF886154 */
  .extern CNPUBSET         /* 0xBF886158 */
  .extern CNPUBINV         /* 0xBF88615C */
  .extern CNPDB            /* 0xBF886160 */
  .extern CNPDBCLR         /* 0xBF886164 */
  .extern CNPDBSET         /* 0xBF886168 */
  .extern CNPDBINV         /* 0xBF88616C */
  .extern CNCONB           /* 0xBF886170 */
  .extern CNCONBCLR        /* 0xBF886174 */
  .extern CNCONBSET        /* 0xBF886178 */
  .extern CNCONBINV        /* 0xBF88617C */
  .extern CNENB            /* 0xBF886180 */
  .extern CNENBCLR         /* 0xBF886184 */
  .extern CNENBSET         /* 0xBF886188 */
  .extern CNENBINV         /* 0xBF88618C */
  .extern CNSTATB          /* 0xBF886190 */
  .extern CNSTATBCLR       /* 0xBF886194 */
  .extern CNSTATBSET       /* 0xBF886198 */
  .extern CNSTATBINV       /* 0xBF88619C */
  .extern DEVCFG3          /* 0xBFC00BF0 */
  .extern DEVCFG2          /* 0xBFC00BF4 */
  .extern DEVCFG1          /* 0xBFC00BF8 */
  .extern DEVCFG0          /* 0xBFC00BFC */
#else
#error Unknown language!
#endif

/* Vector Numbers */
#define _CORE_TIMER_VECTOR                       0
#define _CORE_SOFTWARE_0_VECTOR                  1
#define _CORE_SOFTWARE_1_VECTOR                  2
#define _EXTERNAL_0_VECTOR                       3
#define _TIMER_1_VECTOR                          4
#define _INPUT_CAPTURE_1_VECTOR                  5
#define _OUTPUT_COMPARE_1_VECTOR                 6
#define _EXTERNAL_1_VECTOR                       7
#define _TIMER_2_VECTOR                          8
#define _INPUT_CAPTURE_2_VECTOR                  9
#define _OUTPUT_COMPARE_2_VECTOR                 10
#define _EXTERNAL_2_VECTOR                       11
#define _TIMER_3_VECTOR                          12
#define _INPUT_CAPTURE_3_VECTOR                  13
#define _OUTPUT_COMPARE_3_VECTOR                 14
#define _EXTERNAL_3_VECTOR                       15
#define _TIMER_4_VECTOR                          16
#define _INPUT_CAPTURE_4_VECTOR                  17
#define _OUTPUT_COMPARE_4_VECTOR                 18
#define _EXTERNAL_4_VECTOR                       19
#define _TIMER_5_VECTOR                          20
#define _INPUT_CAPTURE_5_VECTOR                  21
#define _OUTPUT_COMPARE_5_VECTOR                 22
#define _ADC_VECTOR                              23
#define _FAIL_SAFE_MONITOR_VECTOR                24
#define _RTCC_VECTOR                             25
#define _FCE_VECTOR                              26
#define _COMPARATOR_1_VECTOR                     27
#define _COMPARATOR_2_VECTOR                     28
#define _COMPARATOR_3_VECTOR                     29
#define _USB_1_VECTOR                            30
#define _SPI_1_VECTOR                            31
#define _UART_1_VECTOR                           32
#define _I2C_1_VECTOR                            33
#define _CHANGE_NOTICE_VECTOR                    34
#define _PMP_VECTOR                              35
#define _SPI_2_VECTOR                            36
#define _UART_2_VECTOR                           37
#define _I2C_2_VECTOR                            38
#define _CTMU_VECTOR                             39
#define _DMA_0_VECTOR                            40
#define _DMA_1_VECTOR                            41
#define _DMA_2_VECTOR                            42
#define _DMA_3_VECTOR                            43

/* IRQ Numbers */
#define _CORE_TIMER_IRQ                          0
#define _CORE_SOFTWARE_0_IRQ                     1
#define _CORE_SOFTWARE_1_IRQ                     2
#define _EXTERNAL_0_IRQ                          3
#define _TIMER_1_IRQ                             4
#define _INPUT_CAPTURE_ERROR_1_IRQ               5
#define _INPUT_CAPTURE_1_IRQ                     6
#define _OUTPUT_COMPARE_1_IRQ                    7
#define _EXTERNAL_1_IRQ                          8
#define _TIMER_2_IRQ                             9
#define _INPUT_CAPTURE_ERROR_2_IRQ               10
#define _INPUT_CAPTURE_2_IRQ                     11
#define _OUTPUT_COMPARE_2_IRQ                    12
#define _EXTERNAL_2_IRQ                          13
#define _TIMER_3_IRQ                             14
#define _INPUT_CAPTURE_ERROR_3_IRQ               15
#define _INPUT_CAPTURE_3_IRQ                     16
#define _OUTPUT_COMPARE_3_IRQ                    17
#define _EXTERNAL_3_IRQ                          18
#define _TIMER_4_IRQ                             19
#define _INPUT_CAPTURE_ERROR_4_IRQ               20
#define _INPUT_CAPTURE_4_IRQ                     21
#define _OUTPUT_COMPARE_4_IRQ                    22
#define _EXTERNAL_4_IRQ                          23
#define _TIMER_5_IRQ                             24
#define _INPUT_CAPTURE_ERROR_5_IRQ               25
#define _INPUT_CAPTURE_5_IRQ                     26
#define _OUTPUT_COMPARE_5_IRQ                    27
#define _ADC_IRQ                                 28
#define _FAIL_SAFE_MONITOR_IRQ                   29
#define _RTCC_IRQ                                30
#define _FLASH_CONTROL_IRQ                       31
#define _COMPARATOR_1_IRQ                        32
#define _COMPARATOR_2_IRQ                        33
#define _COMPARATOR_3_IRQ                        34
#define _USB_IRQ                                 35
#define _SPI1_ERR_IRQ                            36
#define _SPI1_RX_IRQ                             37
#define _SPI1_TX_IRQ                             38
#define _UART1_ERR_IRQ                           39
#define _UART1_RX_IRQ                            40
#define _UART1_TX_IRQ                            41
#define _I2C1_BUS_IRQ                            42
#define _I2C1_SLAVE_IRQ                          43
#define _I2C1_MASTER_IRQ                         44
#define _CHANGE_NOTICE_A_IRQ                     45
#define _CHANGE_NOTICE_B_IRQ                     46
#define _CHANGE_NOTICE_C_IRQ                     47
#define _PMP_IRQ                                 48
#define _PMP_ERROR_IRQ                           49
#define _SPI2_ERR_IRQ                            50
#define _SPI2_RX_IRQ                             51
#define _SPI2_TX_IRQ                             52
#define _UART2_ERR_IRQ                           53
#define _UART2_RX_IRQ                            54
#define _UART2_TX_IRQ                            55
#define _I2C2_BUS_IRQ                            56
#define _I2C2_SLAVE_IRQ                          57
#define _I2C2_MASTER_IRQ                         58
#define _CTMU_IRQ                                59
#define _DMA0_IRQ                                60
#define _DMA1_IRQ                                61
#define _DMA2_IRQ                                62
#define _DMA3_IRQ                                63

/*  The following device macros indicate which core features are
 *  available on this device.
 */
#ifndef __PIC32_HAS_MIPS32R2
# define __PIC32_HAS_MIPS32R2 1
#endif
#ifndef __PIC32_HAS_MIPS16
# define __PIC32_HAS_MIPS16 1
#endif

#endif
