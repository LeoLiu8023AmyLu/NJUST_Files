//----------------------------------------------------NUST-----------------------------------------------------
// ==============================NUST=================================
// Project Name :   FPGA Home Work    
// File name    :   biss_mode.v
// Library      :   WORK
// Created On   :   2017/12/07 19:13
// Comments     :
// ----------------------------------------------------------------------
// >>>>>>>>>>>>>>>>>>>>>>>>> COPYRIGHT NOTICE <<<<<<<<<<<<<<<<<<<<<<<<<<<
// ----------------------------------------------------------------------
// Copyright 2017 (c) NUST
//
// NUST owns the sole copyright to this software. Under 
// international copyright laws you (1) may not make a copy of this software
// except for the purposes of maintaining a single archive copy, (2) may not 
// derive works herefrom, (3) may not distribute this work to others. These 
// rights are provided for information clarification, other restrictions of 
// rights may apply as well.
// ----------------------------------------------------------------------
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>> Warrantee <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ----------------------------------------------------------------------
// NUST MAKES NO WARRANTY OF ANY KIND WITH REGARD TO THE USE OF
// THIS SOFTWARE, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR
// PURPOSE.
// ----------------------------------------------------------------------
//
// Revision History :
// ----------------------------------------------------------------------
//  Ver :       | Author:   Leo.Liu    | Mod. Date :  2017.12.07     |    
//  Update Detail:                  | 
// ----------------------------------------------------------------------
// OverView
// ========
//
// ----------------------------------------------------------------------
// ======================================================================
`timescale 1ns/100ps
module  biss_mode(
    //====system reset====
    //I_reset_n       ,
    
    //====system clock====
    //I_sys_clk       ,   //100MHz working clock
    
    //====biss interface====
    O_biss_ma       ,
    //I_biss_slo      ,
    
    //====angle signal output====
    O_angle_ena     ,
    O_angle_error   ,
    O_angle_warning ,
    O_angle_crc     ,
    O_angle_data
    );

//------------------------------------------External Signal Definitions------------------------------------------
//====input====
//input           I_reset_n           ;
//input           I_sys_clk           ;
//input           I_biss_slo          ;

//====output====                  
output          O_biss_ma           ;
output          O_angle_ena         ;
output          O_angle_error       ;
output          O_angle_warning     ;
output[5:0]     O_angle_crc         ;
output[25:0]    O_angle_data        ;

//----------------------------------------------Reg Definitions---------------------------------------------------
//====parameter====
parameter       P_STATE_IDLE    =   4'b0001;
parameter       P_STATE_WAIT    =   4'b0010;
parameter       P_STATE_ACK     =   4'b0100;
parameter       P_STATE_WORK    =   4'b1000;

//====output register define====
wire            O_biss_ma           ;
reg             O_angle_warning     ;
reg             O_angle_error       ;
reg             O_angle_ena         ;
reg[25:0]       O_angle_data        ;   // 26 bit data
reg[5:0]        O_angle_crc         ;   // 6  bit crc data

//====internal register define====
reg             R_10mhz_clk          ;
reg[3:0]        R_clk_cnt           ;
wire            W_10mhz_clk          ;
reg[5:0]        R_cnt_biss_cycle    ;
reg[8:0]        R_cnt_100us         ;
reg[3:0]        R_work_state        ;
reg             R_work_ena          ;
reg             R_cnt_ena           ;
reg             R_work_on           ;
reg             R_biss_ma_ena       ;
reg             R_biss_slo 			 ;

//=====test====
reg           I_reset_n           ;
reg           I_sys_clk           ;
reg           I_biss_slo          ;
initial 
begin
    I_reset_n = 0;
    I_sys_clk = 0;
    I_biss_slo = 1'b1;
    #10     I_reset_n = 1'b1;
    #100   I_biss_slo = 1'b0;
    #100    I_biss_slo = 1'b1;
    #1000  I_biss_slo = 1'b0;
    #200  I_biss_slo = 1'b1;
    #200    I_biss_slo = 1'b0;
    #200    I_biss_slo = 1'b1;
end
always #5 I_sys_clk <= ~I_sys_clk;

//---------------------------------------------Main Body of Code---------------------------------------------------
//----------------readme------------------
//
//   Read the angle signal in every
//   100us time
//
//
//----------------readme------------------
//------------------------clock---------------------------
// 10 conter 100MHz --> 10MHz
always @(posedge I_sys_clk or negedge I_reset_n) 
begin
    if (~I_reset_n) 
    begin
        R_clk_cnt   <= 4'd0;
    end
    else
    begin
        if(R_clk_cnt == 4'd4)
        begin
            R_clk_cnt   <= 4'd0;
        end
        else
        begin
            R_clk_cnt <= R_clk_cnt + 4'd1;
        end
    end
end
// Generate 5MHz Clock R_10mhz_clk
always @(posedge I_sys_clk or negedge I_reset_n) 
begin
    if (~I_reset_n) 
    begin
        R_10mhz_clk  <= 1'b0;
    end
    else
    begin
        if(R_clk_cnt == 4'd4)
        begin
            R_10mhz_clk  <= ~R_10mhz_clk;
        end
    end
end
// Buffer change R_10mhz_clk to  W_10mhz_clk
BUFG BUFG_u1 (
    .O(W_10mhz_clk), // 1-bit output: Clock output
    .I(R_10mhz_clk)  // 1-bit input: Clock input
    );
//------------------------clock---------------------------


//-----------------------------------10MHz Clock domain----------------------------------

always @ (negedge W_10mhz_clk or negedge I_reset_n) 
begin
	if(~I_reset_n)
	begin
		R_biss_slo <= 1'b0;
	end
	else
	begin
		R_biss_slo <= I_biss_slo; // Get the signal from I_biss_slo
	end
end
// the 100us counter  
always @(posedge W_10mhz_clk or negedge I_reset_n) 
begin
    if (~I_reset_n) 
    begin
        R_cnt_100us <= 9'b0;
    end
    else
    begin
        R_cnt_100us <= R_cnt_100us + 9'b1;
    end
end
// R_work_ena enable
always @(posedge W_10mhz_clk or negedge I_reset_n) 
begin
    if (~I_reset_n) 
    begin
        R_work_ena  <= 1'b0;
    end
    else
    begin
        //if((&R_cnt_100us) && (R_work_state == P_STATE_IDLE))
        if (R_work_state == P_STATE_IDLE)
        // R_cnt_100us is all 1 and state is IDLE
        begin
            R_work_ena  <= 1'b1;    // to activate the prosses
        end
        else if(~R_biss_ma_ena)
        begin
            R_work_ena  <= 1'b0;
        end
    end
end
// State machine
always @(posedge W_10mhz_clk or negedge I_reset_n) 
begin
    if (~I_reset_n) 
    begin
        R_work_state    <=  P_STATE_IDLE;
        R_biss_ma_ena   <=  1'b1;           //low to work
        R_cnt_ena       <=  1'b0;
    end
    else
    begin
        case(R_work_state)
        P_STATE_IDLE:
        begin
            if(R_work_ena && R_biss_slo)
            begin
                R_work_state    <=  P_STATE_WAIT;
                R_biss_ma_ena   <=  1'b0;
            end
            else
            begin
                R_work_state    <=  P_STATE_IDLE;
                R_biss_ma_ena   <=  1'b1;
            end
        end
        P_STATE_WAIT:
        begin
            if(R_biss_slo)
            begin
                R_work_state    <=  P_STATE_WAIT;
            end
            else
            begin
                R_work_state    <=  P_STATE_ACK;
            end
        end
        P_STATE_ACK:
        begin
            if(R_biss_slo)
            begin
                R_work_state    <=  P_STATE_WORK;
                R_cnt_ena       <=  1'b1;
            end
            else
            begin
                R_work_state    <=  P_STATE_ACK;
                R_cnt_ena       <=  1'b0;
            end
        end
        P_STATE_WORK:
        begin
            if(R_work_on || R_cnt_ena)
            begin
                R_work_state    <=  P_STATE_WORK;
                R_cnt_ena       <=  1'b0;
            end
            else
            begin
                R_work_state    <=  P_STATE_IDLE;
                R_cnt_ena       <=  1'b0;
                R_biss_ma_ena   <=  1'b1;
            end
        end
        default:
        begin
            R_work_state    <=  P_STATE_IDLE;
            R_biss_ma_ena   <=  1'b1;
            R_cnt_ena       <=  1'b0;
        end
        endcase
    end
end
// R_cnt_biss_cycle  11111 6'h3f
always @(posedge W_10mhz_clk or negedge I_reset_n) 
begin
    if (~I_reset_n) 
    begin
        R_cnt_biss_cycle  <= 6'h3f;
    end
    else
    begin
        if(R_cnt_ena)
        begin
            R_cnt_biss_cycle    <= 6'h0;
        end
        else if(~(&R_cnt_biss_cycle))
        begin
            R_cnt_biss_cycle    <= R_cnt_biss_cycle + 6'b1;
        end
    end
end

always @(posedge W_10mhz_clk or negedge I_reset_n) 
begin
    if (~I_reset_n) 
    begin
        R_work_on   <= 1'b0;
    end
    else
    begin
        if(R_cnt_ena)
        begin
            R_work_on   <= 1'b1;
        end
        else if(R_cnt_biss_cycle == 6'd34)
        begin
            R_work_on   <=  1'b0;
        end
    end
end

always @(posedge W_10mhz_clk or negedge I_reset_n) 
begin
    if (~I_reset_n) 
    begin
        O_angle_data    <= 18'b0;
        O_angle_crc     <= 6'b0;
    end
    else
    begin
        case(R_cnt_biss_cycle)
        6'h00:  O_angle_data[25]    <= R_biss_slo;
        6'h01:  O_angle_data[24]    <= R_biss_slo;
        6'h02:  O_angle_data[23]    <= R_biss_slo;
        6'h03:  O_angle_data[22]    <= R_biss_slo;
        6'h04:  O_angle_data[21]    <= R_biss_slo;
        6'h05:  O_angle_data[20]    <= R_biss_slo;
        6'h06:  O_angle_data[19]    <= R_biss_slo;
        6'h07:  O_angle_data[18]    <= R_biss_slo;
        6'h08:  O_angle_data[17]    <= R_biss_slo;
        6'h09:  O_angle_data[16]    <= R_biss_slo;
        6'h0a:  O_angle_data[15]    <= R_biss_slo;
        6'h0b:  O_angle_data[14]    <= R_biss_slo;
        6'h0c:  O_angle_data[13]    <= R_biss_slo;
        6'h0d:  O_angle_data[12]    <= R_biss_slo;
        6'h0e:  O_angle_data[11]    <= R_biss_slo;
        6'h0f:  O_angle_data[10]    <= R_biss_slo;
        6'h10:  O_angle_data[9]     <= R_biss_slo;
        6'h11:  O_angle_data[8]     <= R_biss_slo;
        6'h12:  O_angle_data[7]     <= R_biss_slo;
        6'h13:  O_angle_data[6]     <= R_biss_slo;
        6'h14:  O_angle_data[5]     <= R_biss_slo;
        6'h15:  O_angle_data[4]     <= R_biss_slo;
        6'h16:  O_angle_data[3]     <= R_biss_slo;
        6'h17:  O_angle_data[2]     <= R_biss_slo;
        6'h18:  O_angle_data[1]     <= R_biss_slo;
        6'h19:  O_angle_data[0]     <= R_biss_slo;
        // CRC Data
        6'h1c:  O_angle_crc[5]      <= R_biss_slo;
        6'h1d:  O_angle_crc[4]      <= R_biss_slo;
        6'h1e:  O_angle_crc[3]      <= R_biss_slo;
        6'h1f:  O_angle_crc[2]      <= R_biss_slo;
        6'h20:  O_angle_crc[1]      <= R_biss_slo;
        6'h21:  O_angle_crc[0]      <= R_biss_slo;
        default:;
        endcase
    end
end

always @(posedge W_10mhz_clk or negedge I_reset_n) 
begin
    if (~I_reset_n) 
    begin
        O_angle_ena <= 1'b0;
    end
    else
    begin
        O_angle_ena <= (R_cnt_biss_cycle == 6'd34);
    end
end

always @(posedge W_10mhz_clk or negedge I_reset_n) 
begin
    if (~I_reset_n) 
    begin
        O_angle_error   <= 1'b0;
        O_angle_warning <= 1'b0;
    end
    else
    begin
        if(R_cnt_biss_cycle == 6'h1a)
        begin
            O_angle_error   <= R_biss_slo;
        end
        else if(R_cnt_biss_cycle == 6'h1b)
        begin
            O_angle_warning <= R_biss_slo;
        end
    end
end

assign  O_biss_ma = R_biss_ma_ena || R_10mhz_clk;

endmodule
//====END====