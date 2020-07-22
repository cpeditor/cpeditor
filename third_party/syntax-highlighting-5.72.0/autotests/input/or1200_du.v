//////////////////////////////////////////////////////////////////////
////                                                              ////
////  OR1200's Debug Unit                                         ////
////                                                              ////
////  This file is part of the OpenRISC 1200 project              ////
////  http://www.opencores.org/project,or1k                       ////
////                                                              ////
////  Description                                                 ////
////  Basic OR1200 debug unit.                                    ////
////                                                              ////
////  To Do:                                                      ////
////   - make it smaller and faster                               ////
////                                                              ////
////  Author(s):                                                  ////
////      - Damjan Lampret, lampret@opencores.org                 ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2000 Authors and OPENCORES.ORG                 ////
////                                                              ////
//// This source file may be used and distributed without         ////
//// restriction provided that this copyright statement is not    ////
//// removed from the file and that any derivative work contains  ////
//// the original copyright notice and the associated disclaimer. ////
////                                                              ////
//// This source file is free software; you can redistribute it   ////
//// and/or modify it under the terms of the GNU Lesser General   ////
//// Public License as published by the Free Software Foundation; ////
//// either version 2.1 of the License, or (at your option) any   ////
//// later version.                                               ////
////                                                              ////
//// This source is distributed in the hope that it will be       ////
//// useful, but WITHOUT ANY WARRANTY; without even the implied   ////
//// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      ////
//// PURPOSE.  See the GNU Lesser General Public License for more ////
//// details.                                                     ////
////                                                              ////
//// You should have received a copy of the GNU Lesser General    ////
//// Public License along with this source; if not, download it   ////
//// from http://www.opencores.org/lgpl.shtml                     ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
//
//
// $Log: or1200_du.v,v $
// Revision 2.0  2010/06/30 11:00:00  ORSoC
// Minor update: 
// Bugs fixed. 

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on
`include "or1200_defines.v"

//
// Debug unit
//

module or1200_du(
	// RISC Internal Interface
	clk, rst,
	dcpu_cycstb_i, dcpu_we_i, dcpu_adr_i, dcpu_dat_lsu,
	dcpu_dat_dc, icpu_cycstb_i,
	ex_freeze, branch_op, ex_insn, id_pc,
	spr_dat_npc, rf_dataw,
	du_dsr, du_dmr1, du_stall, du_addr, du_dat_i, du_dat_o,
	du_read, du_write, du_except_stop, du_hwbkpt, du_flush_pipe,
	spr_cs, spr_write, spr_addr, spr_dat_i, spr_dat_o,

	// External Debug Interface
	dbg_stall_i, dbg_ewt_i,	dbg_lss_o, dbg_is_o, dbg_wp_o, dbg_bp_o,
	dbg_stb_i, dbg_we_i, dbg_adr_i, dbg_dat_i, dbg_dat_o, dbg_ack_o
);

parameter dw = `OR1200_OPERAND_WIDTH;
parameter aw = `OR1200_OPERAND_WIDTH;

//
// I/O
//

//
// RISC Internal Interface
//
input				clk;		// Clock
input				rst;		// Reset
input				dcpu_cycstb_i;	// LSU status
input				dcpu_we_i;	// LSU status
input	[31:0]			dcpu_adr_i;	// LSU addr
input	[31:0]			dcpu_dat_lsu;	// LSU store data
input	[31:0]			dcpu_dat_dc;	// LSU load data
input	[`OR1200_FETCHOP_WIDTH-1:0]	icpu_cycstb_i;	// IFETCH unit status
input				ex_freeze;	// EX stage freeze
input	[`OR1200_BRANCHOP_WIDTH-1:0]	branch_op;	// Branch op
input	[dw-1:0]		ex_insn;	// EX insn
input	[31:0]			id_pc;		// insn fetch EA
input	[31:0]			spr_dat_npc;	// Next PC (for trace)
input	[31:0]			rf_dataw;	// ALU result (for trace)
output	[`OR1200_DU_DSR_WIDTH-1:0]     du_dsr;		// DSR
output	[24: 0]			du_dmr1;
output				du_stall;	// Debug Unit Stall
output	[aw-1:0]		du_addr;	// Debug Unit Address
input	[dw-1:0]		du_dat_i;	// Debug Unit Data In
output	[dw-1:0]		du_dat_o;	// Debug Unit Data Out
output				du_read;	// Debug Unit Read Enable
output				du_write;	// Debug Unit Write Enable
input	[13:0]			du_except_stop;	// Exception masked by DSR
output				du_hwbkpt;	// Cause trap exception (HW Breakpoints)
output				du_flush_pipe;	// Cause pipeline flush and pc<-npc
input				spr_cs;		// SPR Chip Select
input				spr_write;	// SPR Read/Write
input	[aw-1:0]		spr_addr;	// SPR Address
input	[dw-1:0]		spr_dat_i;	// SPR Data Input
output	[dw-1:0]		spr_dat_o;	// SPR Data Output

//
// External Debug Interface
//
input			dbg_stall_i;	// External Stall Input
input			dbg_ewt_i;	// External Watchpoint Trigger Input
output	[3:0]		dbg_lss_o;	// External Load/Store Unit Status
output	[1:0]		dbg_is_o;	// External Insn Fetch Status
output	[10:0]		dbg_wp_o;	// Watchpoints Outputs
output			dbg_bp_o;	// Breakpoint Output
input			dbg_stb_i;      // External Address/Data Strobe
input			dbg_we_i;       // External Write Enable
input	[aw-1:0]	dbg_adr_i;	// External Address Input
input	[dw-1:0]	dbg_dat_i;	// External Data Input
output	[dw-1:0]	dbg_dat_o;	// External Data Output
output			dbg_ack_o;	// External Data Acknowledge (not WB compatible)
reg	[dw-1:0]	dbg_dat_o;	// External Data Output
reg			dbg_ack_o;	// External Data Acknowledge (not WB compatible)


//
// Some connections go directly from the CPU through DU to Debug I/F
//
`ifdef OR1200_DU_STATUS_UNIMPLEMENTED
assign dbg_lss_o = 4'b0000;

reg	[1:0]			dbg_is_o;
//
// Show insn activity (temp, must be removed)
//
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dbg_is_o <=  2'b00;
	else if (!ex_freeze & ~((ex_insn[31:26] == `OR1200_OR32_NOP) & ex_insn[16]))
		dbg_is_o <=  ~dbg_is_o;
`ifdef UNUSED
assign dbg_is_o = 2'b00;
`endif
`else
assign dbg_lss_o = dcpu_cycstb_i ? {dcpu_we_i, 3'b000} : 4'b0000;
assign dbg_is_o = {1'b0, icpu_cycstb_i};
`endif
assign dbg_wp_o = 11'b000_0000_0000;

//
// Some connections go directly from Debug I/F through DU to the CPU
//
assign du_stall = dbg_stall_i;
assign du_addr = dbg_adr_i;
assign du_dat_o = dbg_dat_i;
assign du_read = dbg_stb_i && !dbg_we_i;
assign du_write = dbg_stb_i && dbg_we_i;

//
// After a sw breakpoint, the replaced instruction need to be executed.
// We flush the entire pipeline and set the pc to the current address
// to execute the restored address.
//

reg du_flush_pipe_r;
reg dbg_stall_i_r;

assign du_flush_pipe = du_flush_pipe_r;

//
// Register du_flush_pipe
//
always @(posedge clk or `OR1200_RST_EVENT rst) begin
	if (rst == `OR1200_RST_VALUE) begin
		du_flush_pipe_r   <=  1'b0;
	end
	else begin
		du_flush_pipe_r   <=  (dbg_stall_i_r && !dbg_stall_i && |du_except_stop);
	end
end

//
// Detect dbg_stall falling edge
//
always @(posedge clk or `OR1200_RST_EVENT rst) begin
	if (rst == `OR1200_RST_VALUE) begin
		dbg_stall_i_r   <=  1'b0;
	end
	else begin
		dbg_stall_i_r   <=  dbg_stall_i;
	end
end

reg				dbg_ack;
//
// Generate acknowledge -- just delay stb signal
//
always @(posedge clk or `OR1200_RST_EVENT rst) begin
	if (rst == `OR1200_RST_VALUE) begin
		dbg_ack   <=  1'b0;
		dbg_ack_o <=  1'b0;
	end
	else begin
		dbg_ack   <=  dbg_stb_i;		// valid when du_dat_i 
		dbg_ack_o <=  dbg_ack & dbg_stb_i;	// valid when dbg_dat_o 
	end
end

// 
// Register data output
//
always @(posedge clk)
    dbg_dat_o <=  du_dat_i;

`ifdef OR1200_DU_IMPLEMENTED

//
// Debug Mode Register 1
//
`ifdef OR1200_DU_DMR1
reg	[24:0]			dmr1;		// DMR1 implemented
`else
wire	[24:0]			dmr1;		// DMR1 not implemented
`endif
assign du_dmr1 = dmr1;

//
// Debug Mode Register 2
//
`ifdef OR1200_DU_DMR2
reg	[23:0]			dmr2;		// DMR2 implemented
`else
wire	[23:0]			dmr2;		// DMR2 not implemented
`endif

//
// Debug Stop Register
//
`ifdef OR1200_DU_DSR
reg	[`OR1200_DU_DSR_WIDTH-1:0]	dsr;		// DSR implemented
`else
wire	[`OR1200_DU_DSR_WIDTH-1:0]	dsr;		// DSR not implemented
`endif

//
// Debug Reason Register
//
`ifdef OR1200_DU_DRR
reg	[13:0]			drr;		// DRR implemented
`else
wire	[13:0]			drr;		// DRR not implemented
`endif

//
// Debug Value Register N
//
`ifdef OR1200_DU_DVR0
reg	[31:0]			dvr0;
`else
wire	[31:0]			dvr0;
`endif

//
// Debug Value Register N
//
`ifdef OR1200_DU_DVR1
reg	[31:0]			dvr1;
`else
wire	[31:0]			dvr1;
`endif

//
// Debug Value Register N
//
`ifdef OR1200_DU_DVR2
reg	[31:0]			dvr2;
`else
wire	[31:0]			dvr2;
`endif

//
// Debug Value Register N
//
`ifdef OR1200_DU_DVR3
reg	[31:0]			dvr3;
`else
wire	[31:0]			dvr3;
`endif

//
// Debug Value Register N
//
`ifdef OR1200_DU_DVR4
reg	[31:0]			dvr4;
`else
wire	[31:0]			dvr4;
`endif

//
// Debug Value Register N
//
`ifdef OR1200_DU_DVR5
reg	[31:0]			dvr5;
`else
wire	[31:0]			dvr5;
`endif

//
// Debug Value Register N
//
`ifdef OR1200_DU_DVR6
reg	[31:0]			dvr6;
`else
wire	[31:0]			dvr6;
`endif

//
// Debug Value Register N
//
`ifdef OR1200_DU_DVR7
reg	[31:0]			dvr7;
`else
wire	[31:0]			dvr7;
`endif

//
// Debug Control Register N
//
`ifdef OR1200_DU_DCR0
reg	[7:0]			dcr0;
`else
wire	[7:0]			dcr0;
`endif

//
// Debug Control Register N
//
`ifdef OR1200_DU_DCR1
reg	[7:0]			dcr1;
`else
wire	[7:0]			dcr1;
`endif

//
// Debug Control Register N
//
`ifdef OR1200_DU_DCR2
reg	[7:0]			dcr2;
`else
wire	[7:0]			dcr2;
`endif

//
// Debug Control Register N
//
`ifdef OR1200_DU_DCR3
reg	[7:0]			dcr3;
`else
wire	[7:0]			dcr3;
`endif

//
// Debug Control Register N
//
`ifdef OR1200_DU_DCR4
reg	[7:0]			dcr4;
`else
wire	[7:0]			dcr4;
`endif

//
// Debug Control Register N
//
`ifdef OR1200_DU_DCR5
reg	[7:0]			dcr5;
`else
wire	[7:0]			dcr5;
`endif

//
// Debug Control Register N
//
`ifdef OR1200_DU_DCR6
reg	[7:0]			dcr6;
`else
wire	[7:0]			dcr6;
`endif

//
// Debug Control Register N
//
`ifdef OR1200_DU_DCR7
reg	[7:0]			dcr7;
`else
wire	[7:0]			dcr7;
`endif

//
// Debug Watchpoint Counter Register 0
//
`ifdef OR1200_DU_DWCR0
reg	[31:0]			dwcr0;
`else
wire	[31:0]			dwcr0;
`endif

//
// Debug Watchpoint Counter Register 1
//
`ifdef OR1200_DU_DWCR1
reg	[31:0]			dwcr1;
`else
wire	[31:0]			dwcr1;
`endif

//
// Internal wires
//
wire				dmr1_sel; 	// DMR1 select
wire				dmr2_sel; 	// DMR2 select
wire				dsr_sel; 	// DSR select
wire				drr_sel; 	// DRR select
wire				dvr0_sel,
				dvr1_sel,
				dvr2_sel,
				dvr3_sel,
				dvr4_sel,
				dvr5_sel,
				dvr6_sel,
				dvr7_sel; 	// DVR selects
wire				dcr0_sel,
				dcr1_sel,
				dcr2_sel,
				dcr3_sel,
				dcr4_sel,
				dcr5_sel,
				dcr6_sel,
				dcr7_sel; 	// DCR selects
wire				dwcr0_sel,
				dwcr1_sel; 	// DWCR selects
reg				dbg_bp_r;
reg 				ex_freeze_q;
`ifdef OR1200_DU_HWBKPTS
reg	[31:0]			match_cond0_ct;
reg	[31:0]			match_cond1_ct;
reg	[31:0]			match_cond2_ct;
reg	[31:0]			match_cond3_ct;
reg	[31:0]			match_cond4_ct;
reg	[31:0]			match_cond5_ct;
reg	[31:0]			match_cond6_ct;
reg	[31:0]			match_cond7_ct;
reg				match_cond0_stb;
reg				match_cond1_stb;
reg				match_cond2_stb;
reg				match_cond3_stb;
reg				match_cond4_stb;
reg				match_cond5_stb;
reg				match_cond6_stb;
reg				match_cond7_stb;
reg				match0;
reg				match1;
reg				match2;
reg				match3;
reg				match4;
reg				match5;
reg				match6;
reg				match7;
reg				wpcntr0_match;
reg				wpcntr1_match;
reg				incr_wpcntr0;
reg				incr_wpcntr1;
reg	[10:0]			wp;
`endif
wire				du_hwbkpt;
reg				du_hwbkpt_hold;
`ifdef OR1200_DU_READREGS
reg	[31:0]			spr_dat_o;
`endif
reg	[13:0]			except_stop;	// Exceptions that stop because of DSR
`ifdef OR1200_DU_TB_IMPLEMENTED
wire				tb_enw;
reg	[7:0]			tb_wadr;
reg [31:0]			tb_timstmp;
`endif
wire	[31:0]			tbia_dat_o;
wire	[31:0]			tbim_dat_o;
wire	[31:0]			tbar_dat_o;
wire	[31:0]			tbts_dat_o;

//
// DU registers address decoder
//
`ifdef OR1200_DU_DMR1
assign dmr1_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DMR1));
`endif
`ifdef OR1200_DU_DMR2
assign dmr2_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DMR2));
`endif
`ifdef OR1200_DU_DSR
assign dsr_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DSR));
`endif
`ifdef OR1200_DU_DRR
assign drr_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DRR));
`endif
`ifdef OR1200_DU_DVR0
assign dvr0_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DVR0));
`endif
`ifdef OR1200_DU_DVR1
assign dvr1_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DVR1));
`endif
`ifdef OR1200_DU_DVR2
assign dvr2_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DVR2));
`endif
`ifdef OR1200_DU_DVR3
assign dvr3_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DVR3));
`endif
`ifdef OR1200_DU_DVR4
assign dvr4_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DVR4));
`endif
`ifdef OR1200_DU_DVR5
assign dvr5_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DVR5));
`endif
`ifdef OR1200_DU_DVR6
assign dvr6_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DVR6));
`endif
`ifdef OR1200_DU_DVR7
assign dvr7_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DVR7));
`endif
`ifdef OR1200_DU_DCR0
assign dcr0_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DCR0));
`endif
`ifdef OR1200_DU_DCR1
assign dcr1_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DCR1));
`endif
`ifdef OR1200_DU_DCR2
assign dcr2_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DCR2));
`endif
`ifdef OR1200_DU_DCR3
assign dcr3_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DCR3));
`endif
`ifdef OR1200_DU_DCR4
assign dcr4_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DCR4));
`endif
`ifdef OR1200_DU_DCR5
assign dcr5_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DCR5));
`endif
`ifdef OR1200_DU_DCR6
assign dcr6_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DCR6));
`endif
`ifdef OR1200_DU_DCR7
assign dcr7_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DCR7));
`endif
`ifdef OR1200_DU_DWCR0
assign dwcr0_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DWCR0));
`endif
`ifdef OR1200_DU_DWCR1
assign dwcr1_sel = (spr_cs && (spr_addr[`OR1200_DUOFS_BITS] == `OR1200_DU_DWCR1));
`endif

// Track previous ex_freeze to detect when signals are updated
always @(posedge clk)
  ex_freeze_q <= ex_freeze;

//
// Decode started exception
//
// du_except_stop comes from or1200_except
//   
always @(du_except_stop or ex_freeze_q) begin
	except_stop = 14'b00_0000_0000_0000;
	casez (du_except_stop)
	        14'b1?_????_????_????:
			except_stop[`OR1200_DU_DRR_TTE] = 1'b1;
		14'b01_????_????_????: begin
			except_stop[`OR1200_DU_DRR_IE] = 1'b1;
		end
		14'b00_1???_????_????: begin
			except_stop[`OR1200_DU_DRR_IME] = 1'b1;
		end
		14'b00_01??_????_????:
			except_stop[`OR1200_DU_DRR_IPFE] = 1'b1;
		14'b00_001?_????_????: begin
			except_stop[`OR1200_DU_DRR_BUSEE] = 1'b1;
		end
		14'b00_0001_????_????:
			except_stop[`OR1200_DU_DRR_IIE] = 1'b1;
		14'b00_0000_1???_????: begin
			except_stop[`OR1200_DU_DRR_AE] = 1'b1;
		end
		14'b00_0000_01??_????: begin
			except_stop[`OR1200_DU_DRR_DME] = 1'b1;
		end
		14'b00_0000_001?_????:
			except_stop[`OR1200_DU_DRR_DPFE] = 1'b1;
		14'b00_0000_0001_????:
			except_stop[`OR1200_DU_DRR_BUSEE] = 1'b1;
		14'b00_0000_0000_1???: begin
			except_stop[`OR1200_DU_DRR_RE] = 1'b1;
		end
		14'b00_0000_0000_01??: begin
			except_stop[`OR1200_DU_DRR_TE] = 1'b1 & ~ex_freeze_q;
		end
		14'b00_0000_0000_001?: begin
		        except_stop[`OR1200_DU_DRR_FPE] = 1'b1;
		end	  
		14'b00_0000_0000_0001:
			except_stop[`OR1200_DU_DRR_SCE] = 1'b1 & ~ex_freeze_q;
		default:
			except_stop = 14'b00_0000_0000_0000;
	endcase // casez (du_except_stop)
end

//
// dbg_bp_o is registered
//
assign dbg_bp_o = dbg_bp_r;

//
// Breakpoint activation register
//
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dbg_bp_r <=  1'b0;
	else if (!ex_freeze)
		dbg_bp_r <=  |except_stop
`ifdef OR1200_DU_DMR1_ST
                        | ~((ex_insn[31:26] == `OR1200_OR32_NOP) & ex_insn[16]) & dmr1[`OR1200_DU_DMR1_ST]
`endif
`ifdef OR1200_DU_DMR1_BT
                        | (branch_op != `OR1200_BRANCHOP_NOP) & (branch_op != `OR1200_BRANCHOP_RFE) & dmr1[`OR1200_DU_DMR1_BT]
`endif
			;
        else
                dbg_bp_r <=  |except_stop;

//
// Write to DMR1
//
`ifdef OR1200_DU_DMR1
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dmr1 <= 25'h000_0000;
	else if (dmr1_sel && spr_write)
`ifdef OR1200_DU_HWBKPTS
		dmr1 <=  spr_dat_i[24:0];
`else
		dmr1 <=  {1'b0, spr_dat_i[23:22], 22'h00_0000};
`endif
`else
assign dmr1 = 25'h000_0000;
`endif

//
// Write to DMR2
//
`ifdef OR1200_DU_DMR2
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dmr2 <= 24'h00_0000;
	else if (dmr2_sel && spr_write)
		dmr2 <=  spr_dat_i[23:0];
`else
assign dmr2 = 24'h00_0000;
`endif

//
// Write to DSR
//
`ifdef OR1200_DU_DSR
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dsr <= {`OR1200_DU_DSR_WIDTH{1'b0}};
	else if (dsr_sel && spr_write)
		dsr <=  spr_dat_i[`OR1200_DU_DSR_WIDTH-1:0];
`else
assign dsr = {`OR1200_DU_DSR_WIDTH{1'b0}};
`endif

//
// Write to DRR
//
`ifdef OR1200_DU_DRR
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		drr <= 14'b0;
	else if (drr_sel && spr_write)
		drr <=  spr_dat_i[13:0];
	else
		drr <=  drr | except_stop;
`else
assign drr = 14'b0;
`endif

//
// Write to DVR0
//
`ifdef OR1200_DU_DVR0
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dvr0 <= 32'h0000_0000;
	else if (dvr0_sel && spr_write)
		dvr0 <=  spr_dat_i[31:0];
`else
assign dvr0 = 32'h0000_0000;
`endif

//
// Write to DVR1
//
`ifdef OR1200_DU_DVR1
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dvr1 <= 32'h0000_0000;
	else if (dvr1_sel && spr_write)
		dvr1 <=  spr_dat_i[31:0];
`else
assign dvr1 = 32'h0000_0000;
`endif

//
// Write to DVR2
//
`ifdef OR1200_DU_DVR2
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dvr2 <= 32'h0000_0000;
	else if (dvr2_sel && spr_write)
		dvr2 <=  spr_dat_i[31:0];
`else
assign dvr2 = 32'h0000_0000;
`endif

//
// Write to DVR3
//
`ifdef OR1200_DU_DVR3
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dvr3 <= 32'h0000_0000;
	else if (dvr3_sel && spr_write)
		dvr3 <=  spr_dat_i[31:0];
`else
assign dvr3 = 32'h0000_0000;
`endif

//
// Write to DVR4
//
`ifdef OR1200_DU_DVR4
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dvr4 <= 32'h0000_0000;
	else if (dvr4_sel && spr_write)
		dvr4 <=  spr_dat_i[31:0];
`else
assign dvr4 = 32'h0000_0000;
`endif

//
// Write to DVR5
//
`ifdef OR1200_DU_DVR5
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dvr5 <= 32'h0000_0000;
	else if (dvr5_sel && spr_write)
		dvr5 <=  spr_dat_i[31:0];
`else
assign dvr5 = 32'h0000_0000;
`endif

//
// Write to DVR6
//
`ifdef OR1200_DU_DVR6
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dvr6 <= 32'h0000_0000;
	else if (dvr6_sel && spr_write)
		dvr6 <=  spr_dat_i[31:0];
`else
assign dvr6 = 32'h0000_0000;
`endif

//
// Write to DVR7
//
`ifdef OR1200_DU_DVR7
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dvr7 <= 32'h0000_0000;
	else if (dvr7_sel && spr_write)
		dvr7 <=  spr_dat_i[31:0];
`else
assign dvr7 = 32'h0000_0000;
`endif

//
// Write to DCR0
//
`ifdef OR1200_DU_DCR0
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dcr0 <= 8'h00;
	else if (dcr0_sel && spr_write)
		dcr0 <=  spr_dat_i[7:0];
`else
assign dcr0 = 8'h00;
`endif

//
// Write to DCR1
//
`ifdef OR1200_DU_DCR1
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dcr1 <= 8'h00;
	else if (dcr1_sel && spr_write)
		dcr1 <=  spr_dat_i[7:0];
`else
assign dcr1 = 8'h00;
`endif

//
// Write to DCR2
//
`ifdef OR1200_DU_DCR2
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dcr2 <= 8'h00;
	else if (dcr2_sel && spr_write)
		dcr2 <=  spr_dat_i[7:0];
`else
assign dcr2 = 8'h00;
`endif

//
// Write to DCR3
//
`ifdef OR1200_DU_DCR3
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dcr3 <= 8'h00;
	else if (dcr3_sel && spr_write)
		dcr3 <=  spr_dat_i[7:0];
`else
assign dcr3 = 8'h00;
`endif

//
// Write to DCR4
//
`ifdef OR1200_DU_DCR4
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dcr4 <= 8'h00;
	else if (dcr4_sel && spr_write)
		dcr4 <=  spr_dat_i[7:0];
`else
assign dcr4 = 8'h00;
`endif

//
// Write to DCR5
//
`ifdef OR1200_DU_DCR5
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dcr5 <= 8'h00;
	else if (dcr5_sel && spr_write)
		dcr5 <=  spr_dat_i[7:0];
`else
assign dcr5 = 8'h00;
`endif

//
// Write to DCR6
//
`ifdef OR1200_DU_DCR6
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dcr6 <= 8'h00;
	else if (dcr6_sel && spr_write)
		dcr6 <=  spr_dat_i[7:0];
`else
assign dcr6 = 8'h00;
`endif

//
// Write to DCR7
//
`ifdef OR1200_DU_DCR7
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dcr7 <= 8'h00;
	else if (dcr7_sel && spr_write)
		dcr7 <=  spr_dat_i[7:0];
`else
assign dcr7 = 8'h00;
`endif

//
// Write to DWCR0
//
`ifdef OR1200_DU_DWCR0
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dwcr0 <= 32'h0000_0000;
	else if (dwcr0_sel && spr_write)
		dwcr0 <=  spr_dat_i[31:0];
	else if (incr_wpcntr0)
		dwcr0[`OR1200_DU_DWCR_COUNT] <=  dwcr0[`OR1200_DU_DWCR_COUNT] + 16'h0001;
`else
assign dwcr0 = 32'h0000_0000;
`endif

//
// Write to DWCR1
//
`ifdef OR1200_DU_DWCR1
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		dwcr1 <= 32'h0000_0000;
	else if (dwcr1_sel && spr_write)
		dwcr1 <=  spr_dat_i[31:0];
	else if (incr_wpcntr1)
		dwcr1[`OR1200_DU_DWCR_COUNT] <=  dwcr1[`OR1200_DU_DWCR_COUNT] + 16'h0001;
`else
assign dwcr1 = 32'h0000_0000;
`endif

//
// Read DU registers
//
`ifdef OR1200_DU_READREGS
always @(spr_addr or dsr or drr or dmr1 or dmr2
	or dvr0 or dvr1 or dvr2 or dvr3 or dvr4
	or dvr5 or dvr6 or dvr7
	or dcr0 or dcr1 or dcr2 or dcr3 or dcr4
	or dcr5 or dcr6 or dcr7
	or dwcr0 or dwcr1
`ifdef OR1200_DU_TB_IMPLEMENTED
	or tb_wadr or tbia_dat_o or tbim_dat_o
	or tbar_dat_o or tbts_dat_o
`endif
	)
	casez (spr_addr[`OR1200_DUOFS_BITS]) // synopsys parallel_case
`ifdef OR1200_DU_DVR0
		`OR1200_DU_DVR0:
			spr_dat_o = dvr0;
`endif
`ifdef OR1200_DU_DVR1
		`OR1200_DU_DVR1:
			spr_dat_o = dvr1;
`endif
`ifdef OR1200_DU_DVR2
		`OR1200_DU_DVR2:
			spr_dat_o = dvr2;
`endif
`ifdef OR1200_DU_DVR3
		`OR1200_DU_DVR3:
			spr_dat_o = dvr3;
`endif
`ifdef OR1200_DU_DVR4
		`OR1200_DU_DVR4:
			spr_dat_o = dvr4;
`endif
`ifdef OR1200_DU_DVR5
		`OR1200_DU_DVR5:
			spr_dat_o = dvr5;
`endif
`ifdef OR1200_DU_DVR6
		`OR1200_DU_DVR6:
			spr_dat_o = dvr6;
`endif
`ifdef OR1200_DU_DVR7
		`OR1200_DU_DVR7:
			spr_dat_o = dvr7;
`endif
`ifdef OR1200_DU_DCR0
		`OR1200_DU_DCR0:
			spr_dat_o = {24'h00_0000, dcr0};
`endif
`ifdef OR1200_DU_DCR1
		`OR1200_DU_DCR1:
			spr_dat_o = {24'h00_0000, dcr1};
`endif
`ifdef OR1200_DU_DCR2
		`OR1200_DU_DCR2:
			spr_dat_o = {24'h00_0000, dcr2};
`endif
`ifdef OR1200_DU_DCR3
		`OR1200_DU_DCR3:
			spr_dat_o = {24'h00_0000, dcr3};
`endif
`ifdef OR1200_DU_DCR4
		`OR1200_DU_DCR4:
			spr_dat_o = {24'h00_0000, dcr4};
`endif
`ifdef OR1200_DU_DCR5
		`OR1200_DU_DCR5:
			spr_dat_o = {24'h00_0000, dcr5};
`endif
`ifdef OR1200_DU_DCR6
		`OR1200_DU_DCR6:
			spr_dat_o = {24'h00_0000, dcr6};
`endif
`ifdef OR1200_DU_DCR7
		`OR1200_DU_DCR7:
			spr_dat_o = {24'h00_0000, dcr7};
`endif
`ifdef OR1200_DU_DMR1
		`OR1200_DU_DMR1:
			spr_dat_o = {7'h00, dmr1};
`endif
`ifdef OR1200_DU_DMR2
		`OR1200_DU_DMR2:
			spr_dat_o = {8'h00, dmr2};
`endif
`ifdef OR1200_DU_DWCR0
		`OR1200_DU_DWCR0:
			spr_dat_o = dwcr0;
`endif
`ifdef OR1200_DU_DWCR1
		`OR1200_DU_DWCR1:
			spr_dat_o = dwcr1;
`endif
`ifdef OR1200_DU_DSR
		`OR1200_DU_DSR:
			spr_dat_o = {18'b0, dsr};
`endif
`ifdef OR1200_DU_DRR
		`OR1200_DU_DRR:
			spr_dat_o = {18'b0, drr};
`endif
`ifdef OR1200_DU_TB_IMPLEMENTED
		`OR1200_DU_TBADR:
			spr_dat_o = {24'h000000, tb_wadr};
		`OR1200_DU_TBIA:
			spr_dat_o = tbia_dat_o;
		`OR1200_DU_TBIM:
			spr_dat_o = tbim_dat_o;
		`OR1200_DU_TBAR:
			spr_dat_o = tbar_dat_o;
		`OR1200_DU_TBTS:
			spr_dat_o = tbts_dat_o;
`endif
		default:
			spr_dat_o = 32'h0000_0000;
	endcase
`endif

//
// DSR alias
//
assign du_dsr = dsr;

`ifdef OR1200_DU_HWBKPTS

//
// Compare To What (Match Condition 0)
//
always @(dcr0 or id_pc or dcpu_adr_i or dcpu_dat_dc
	or dcpu_dat_lsu or dcpu_we_i)
	case (dcr0[`OR1200_DU_DCR_CT])		// synopsys parallel_case
		3'b001:	match_cond0_ct = id_pc;		// insn fetch EA
		3'b010:	match_cond0_ct = dcpu_adr_i;	// load EA
		3'b011:	match_cond0_ct = dcpu_adr_i;	// store EA
		3'b100:	match_cond0_ct = dcpu_dat_dc;	// load data
		3'b101:	match_cond0_ct = dcpu_dat_lsu;	// store data
		3'b110:	match_cond0_ct = dcpu_adr_i;	// load/store EA
		default:match_cond0_ct = dcpu_we_i ? dcpu_dat_lsu : dcpu_dat_dc;
	endcase

//
// When To Compare (Match Condition 0)
//
always @(dcr0 or dcpu_cycstb_i)
	case (dcr0[`OR1200_DU_DCR_CT]) 		// synopsys parallel_case
		3'b000:	match_cond0_stb = 1'b0;		//comparison disabled
		3'b001:	match_cond0_stb = 1'b1;		// insn fetch EA
		default:match_cond0_stb = dcpu_cycstb_i; // any load/store
	endcase

//
// Match Condition 0
//
always @(match_cond0_stb or dcr0 or dvr0 or match_cond0_ct)
	casex ({match_cond0_stb, dcr0[`OR1200_DU_DCR_CC]})
		4'b0_xxx,
		4'b1_000,
		4'b1_111: match0 = 1'b0;
		4'b1_001: match0 =
			({(match_cond0_ct[31] ^ dcr0[`OR1200_DU_DCR_SC]), match_cond0_ct[30:0]} ==
			 {(dvr0[31] ^ dcr0[`OR1200_DU_DCR_SC]), dvr0[30:0]});
		4'b1_010: match0 = 
			({(match_cond0_ct[31] ^ dcr0[`OR1200_DU_DCR_SC]), match_cond0_ct[30:0]} <
			 {(dvr0[31] ^ dcr0[`OR1200_DU_DCR_SC]), dvr0[30:0]});
		4'b1_011: match0 = 
			({(match_cond0_ct[31] ^ dcr0[`OR1200_DU_DCR_SC]), match_cond0_ct[30:0]} <=
			 {(dvr0[31] ^ dcr0[`OR1200_DU_DCR_SC]), dvr0[30:0]});
		4'b1_100: match0 = 
			({(match_cond0_ct[31] ^ dcr0[`OR1200_DU_DCR_SC]), match_cond0_ct[30:0]} >
			 {(dvr0[31] ^ dcr0[`OR1200_DU_DCR_SC]), dvr0[30:0]});
		4'b1_101: match0 = 
			({(match_cond0_ct[31] ^ dcr0[`OR1200_DU_DCR_SC]), match_cond0_ct[30:0]} >=
			 {(dvr0[31] ^ dcr0[`OR1200_DU_DCR_SC]), dvr0[30:0]});
		4'b1_110: match0 = 
			({(match_cond0_ct[31] ^ dcr0[`OR1200_DU_DCR_SC]), match_cond0_ct[30:0]} !=
			 {(dvr0[31] ^ dcr0[`OR1200_DU_DCR_SC]), dvr0[30:0]});
	endcase

//
// Watchpoint 0
//
always @(dmr1 or match0)
	case (dmr1[`OR1200_DU_DMR1_CW0])
		2'b00: wp[0] = match0;
		2'b01: wp[0] = match0;
		2'b10: wp[0] = match0;
		2'b11: wp[0] = 1'b0;
	endcase

//
// Compare To What (Match Condition 1)
//
always @(dcr1 or id_pc or dcpu_adr_i or dcpu_dat_dc
	or dcpu_dat_lsu or dcpu_we_i)
	case (dcr1[`OR1200_DU_DCR_CT])		// synopsys parallel_case
		3'b001:	match_cond1_ct = id_pc;		// insn fetch EA
		3'b010:	match_cond1_ct = dcpu_adr_i;	// load EA
		3'b011:	match_cond1_ct = dcpu_adr_i;	// store EA
		3'b100:	match_cond1_ct = dcpu_dat_dc;	// load data
		3'b101:	match_cond1_ct = dcpu_dat_lsu;	// store data
		3'b110:	match_cond1_ct = dcpu_adr_i;	// load/store EA
		default:match_cond1_ct = dcpu_we_i ? dcpu_dat_lsu : dcpu_dat_dc;
	endcase

//
// When To Compare (Match Condition 1)
//
always @(dcr1 or dcpu_cycstb_i)
	case (dcr1[`OR1200_DU_DCR_CT]) 		// synopsys parallel_case
		3'b000:	match_cond1_stb = 1'b0;		//comparison disabled
		3'b001:	match_cond1_stb = 1'b1;		// insn fetch EA
		default:match_cond1_stb = dcpu_cycstb_i; // any load/store
	endcase

//
// Match Condition 1
//
always @(match_cond1_stb or dcr1 or dvr1 or match_cond1_ct)
	casex ({match_cond1_stb, dcr1[`OR1200_DU_DCR_CC]})
		4'b0_xxx,
		4'b1_000,
		4'b1_111: match1 = 1'b0;
		4'b1_001: match1 =
			({(match_cond1_ct[31] ^ dcr1[`OR1200_DU_DCR_SC]), match_cond1_ct[30:0]} ==
			 {(dvr1[31] ^ dcr1[`OR1200_DU_DCR_SC]), dvr1[30:0]});
		4'b1_010: match1 = 
			({(match_cond1_ct[31] ^ dcr1[`OR1200_DU_DCR_SC]), match_cond1_ct[30:0]} <
			 {(dvr1[31] ^ dcr1[`OR1200_DU_DCR_SC]), dvr1[30:0]});
		4'b1_011: match1 = 
			({(match_cond1_ct[31] ^ dcr1[`OR1200_DU_DCR_SC]), match_cond1_ct[30:0]} <=
			 {(dvr1[31] ^ dcr1[`OR1200_DU_DCR_SC]), dvr1[30:0]});
		4'b1_100: match1 = 
			({(match_cond1_ct[31] ^ dcr1[`OR1200_DU_DCR_SC]), match_cond1_ct[30:0]} >
			 {(dvr1[31] ^ dcr1[`OR1200_DU_DCR_SC]), dvr1[30:0]});
		4'b1_101: match1 = 
			({(match_cond1_ct[31] ^ dcr1[`OR1200_DU_DCR_SC]), match_cond1_ct[30:0]} >=
			 {(dvr1[31] ^ dcr1[`OR1200_DU_DCR_SC]), dvr1[30:0]});
		4'b1_110: match1 = 
			({(match_cond1_ct[31] ^ dcr1[`OR1200_DU_DCR_SC]), match_cond1_ct[30:0]} !=
			 {(dvr1[31] ^ dcr1[`OR1200_DU_DCR_SC]), dvr1[30:0]});
	endcase

//
// Watchpoint 1
//
always @(dmr1 or match1 or wp)
	case (dmr1[`OR1200_DU_DMR1_CW1])
		2'b00: wp[1] = match1;
		2'b01: wp[1] = match1 & wp[0];
		2'b10: wp[1] = match1 | wp[0];
		2'b11: wp[1] = 1'b0;
	endcase

//
// Compare To What (Match Condition 2)
//
always @(dcr2 or id_pc or dcpu_adr_i or dcpu_dat_dc
	or dcpu_dat_lsu or dcpu_we_i)
	case (dcr2[`OR1200_DU_DCR_CT])		// synopsys parallel_case
		3'b001:	match_cond2_ct = id_pc;		// insn fetch EA
		3'b010:	match_cond2_ct = dcpu_adr_i;	// load EA
		3'b011:	match_cond2_ct = dcpu_adr_i;	// store EA
		3'b100:	match_cond2_ct = dcpu_dat_dc;	// load data
		3'b101:	match_cond2_ct = dcpu_dat_lsu;	// store data
		3'b110:	match_cond2_ct = dcpu_adr_i;	// load/store EA
		default:match_cond2_ct = dcpu_we_i ? dcpu_dat_lsu : dcpu_dat_dc;
	endcase

//
// When To Compare (Match Condition 2)
//
always @(dcr2 or dcpu_cycstb_i)
	case (dcr2[`OR1200_DU_DCR_CT]) 		// synopsys parallel_case
		3'b000:	match_cond2_stb = 1'b0;		//comparison disabled
		3'b001:	match_cond2_stb = 1'b1;		// insn fetch EA
		default:match_cond2_stb = dcpu_cycstb_i; // any load/store
	endcase

//
// Match Condition 2
//
always @(match_cond2_stb or dcr2 or dvr2 or match_cond2_ct)
	casex ({match_cond2_stb, dcr2[`OR1200_DU_DCR_CC]})
		4'b0_xxx,
		4'b1_000,
		4'b1_111: match2 = 1'b0;
		4'b1_001: match2 =
			({(match_cond2_ct[31] ^ dcr2[`OR1200_DU_DCR_SC]), match_cond2_ct[30:0]} ==
			 {(dvr2[31] ^ dcr2[`OR1200_DU_DCR_SC]), dvr2[30:0]});
		4'b1_010: match2 = 
			({(match_cond2_ct[31] ^ dcr2[`OR1200_DU_DCR_SC]), match_cond2_ct[30:0]} <
			 {(dvr2[31] ^ dcr2[`OR1200_DU_DCR_SC]), dvr2[30:0]});
		4'b1_011: match2 = 
			({(match_cond2_ct[31] ^ dcr2[`OR1200_DU_DCR_SC]), match_cond2_ct[30:0]} <=
			 {(dvr2[31] ^ dcr2[`OR1200_DU_DCR_SC]), dvr2[30:0]});
		4'b1_100: match2 = 
			({(match_cond2_ct[31] ^ dcr2[`OR1200_DU_DCR_SC]), match_cond2_ct[30:0]} >
			 {(dvr2[31] ^ dcr2[`OR1200_DU_DCR_SC]), dvr2[30:0]});
		4'b1_101: match2 = 
			({(match_cond2_ct[31] ^ dcr2[`OR1200_DU_DCR_SC]), match_cond2_ct[30:0]} >=
			 {(dvr2[31] ^ dcr2[`OR1200_DU_DCR_SC]), dvr2[30:0]});
		4'b1_110: match2 = 
			({(match_cond2_ct[31] ^ dcr2[`OR1200_DU_DCR_SC]), match_cond2_ct[30:0]} !=
			 {(dvr2[31] ^ dcr2[`OR1200_DU_DCR_SC]), dvr2[30:0]});
	endcase

//
// Watchpoint 2
//
always @(dmr1 or match2 or wp)
	case (dmr1[`OR1200_DU_DMR1_CW2])
		2'b00: wp[2] = match2;
		2'b01: wp[2] = match2 & wp[1];
		2'b10: wp[2] = match2 | wp[1];
		2'b11: wp[2] = 1'b0;
	endcase

//
// Compare To What (Match Condition 3)
//
always @(dcr3 or id_pc or dcpu_adr_i or dcpu_dat_dc
	or dcpu_dat_lsu or dcpu_we_i)
	case (dcr3[`OR1200_DU_DCR_CT])		// synopsys parallel_case
		3'b001:	match_cond3_ct = id_pc;		// insn fetch EA
		3'b010:	match_cond3_ct = dcpu_adr_i;	// load EA
		3'b011:	match_cond3_ct = dcpu_adr_i;	// store EA
		3'b100:	match_cond3_ct = dcpu_dat_dc;	// load data
		3'b101:	match_cond3_ct = dcpu_dat_lsu;	// store data
		3'b110:	match_cond3_ct = dcpu_adr_i;	// load/store EA
		default:match_cond3_ct = dcpu_we_i ? dcpu_dat_lsu : dcpu_dat_dc;
	endcase

//
// When To Compare (Match Condition 3)
//
always @(dcr3 or dcpu_cycstb_i)
	case (dcr3[`OR1200_DU_DCR_CT]) 		// synopsys parallel_case
		3'b000:	match_cond3_stb = 1'b0;		//comparison disabled
		3'b001:	match_cond3_stb = 1'b1;		// insn fetch EA
		default:match_cond3_stb = dcpu_cycstb_i; // any load/store
	endcase

//
// Match Condition 3
//
always @(match_cond3_stb or dcr3 or dvr3 or match_cond3_ct)
	casex ({match_cond3_stb, dcr3[`OR1200_DU_DCR_CC]})
		4'b0_xxx,
		4'b1_000,
		4'b1_111: match3 = 1'b0;
		4'b1_001: match3 =
			({(match_cond3_ct[31] ^ dcr3[`OR1200_DU_DCR_SC]), match_cond3_ct[30:0]} ==
			 {(dvr3[31] ^ dcr3[`OR1200_DU_DCR_SC]), dvr3[30:0]});
		4'b1_010: match3 = 
			({(match_cond3_ct[31] ^ dcr3[`OR1200_DU_DCR_SC]), match_cond3_ct[30:0]} <
			 {(dvr3[31] ^ dcr3[`OR1200_DU_DCR_SC]), dvr3[30:0]});
		4'b1_011: match3 = 
			({(match_cond3_ct[31] ^ dcr3[`OR1200_DU_DCR_SC]), match_cond3_ct[30:0]} <=
			 {(dvr3[31] ^ dcr3[`OR1200_DU_DCR_SC]), dvr3[30:0]});
		4'b1_100: match3 = 
			({(match_cond3_ct[31] ^ dcr3[`OR1200_DU_DCR_SC]), match_cond3_ct[30:0]} >
			 {(dvr3[31] ^ dcr3[`OR1200_DU_DCR_SC]), dvr3[30:0]});
		4'b1_101: match3 = 
			({(match_cond3_ct[31] ^ dcr3[`OR1200_DU_DCR_SC]), match_cond3_ct[30:0]} >=
			 {(dvr3[31] ^ dcr3[`OR1200_DU_DCR_SC]), dvr3[30:0]});
		4'b1_110: match3 = 
			({(match_cond3_ct[31] ^ dcr3[`OR1200_DU_DCR_SC]), match_cond3_ct[30:0]} !=
			 {(dvr3[31] ^ dcr3[`OR1200_DU_DCR_SC]), dvr3[30:0]});
	endcase

//
// Watchpoint 3
//
always @(dmr1 or match3 or wp)
	case (dmr1[`OR1200_DU_DMR1_CW3])
		2'b00: wp[3] = match3;
		2'b01: wp[3] = match3 & wp[2];
		2'b10: wp[3] = match3 | wp[2];
		2'b11: wp[3] = 1'b0;
	endcase

//
// Compare To What (Match Condition 4)
//
always @(dcr4 or id_pc or dcpu_adr_i or dcpu_dat_dc
	or dcpu_dat_lsu or dcpu_we_i)
	case (dcr4[`OR1200_DU_DCR_CT])		// synopsys parallel_case
		3'b001:	match_cond4_ct = id_pc;		// insn fetch EA
		3'b010:	match_cond4_ct = dcpu_adr_i;	// load EA
		3'b011:	match_cond4_ct = dcpu_adr_i;	// store EA
		3'b100:	match_cond4_ct = dcpu_dat_dc;	// load data
		3'b101:	match_cond4_ct = dcpu_dat_lsu;	// store data
		3'b110:	match_cond4_ct = dcpu_adr_i;	// load/store EA
		default:match_cond4_ct = dcpu_we_i ? dcpu_dat_lsu : dcpu_dat_dc;
	endcase

//
// When To Compare (Match Condition 4)
//
always @(dcr4 or dcpu_cycstb_i)
	case (dcr4[`OR1200_DU_DCR_CT]) 		// synopsys parallel_case
		3'b000:	match_cond4_stb = 1'b0;		//comparison disabled
		3'b001:	match_cond4_stb = 1'b1;		// insn fetch EA
		default:match_cond4_stb = dcpu_cycstb_i; // any load/store
	endcase

//
// Match Condition 4
//
always @(match_cond4_stb or dcr4 or dvr4 or match_cond4_ct)
	casex ({match_cond4_stb, dcr4[`OR1200_DU_DCR_CC]})
		4'b0_xxx,
		4'b1_000,
		4'b1_111: match4 = 1'b0;
		4'b1_001: match4 =
			({(match_cond4_ct[31] ^ dcr4[`OR1200_DU_DCR_SC]), match_cond4_ct[30:0]} ==
			 {(dvr4[31] ^ dcr4[`OR1200_DU_DCR_SC]), dvr4[30:0]});
		4'b1_010: match4 = 
			({(match_cond4_ct[31] ^ dcr4[`OR1200_DU_DCR_SC]), match_cond4_ct[30:0]} <
			 {(dvr4[31] ^ dcr4[`OR1200_DU_DCR_SC]), dvr4[30:0]});
		4'b1_011: match4 = 
			({(match_cond4_ct[31] ^ dcr4[`OR1200_DU_DCR_SC]), match_cond4_ct[30:0]} <=
			 {(dvr4[31] ^ dcr4[`OR1200_DU_DCR_SC]), dvr4[30:0]});
		4'b1_100: match4 = 
			({(match_cond4_ct[31] ^ dcr4[`OR1200_DU_DCR_SC]), match_cond4_ct[30:0]} >
			 {(dvr4[31] ^ dcr4[`OR1200_DU_DCR_SC]), dvr4[30:0]});
		4'b1_101: match4 = 
			({(match_cond4_ct[31] ^ dcr4[`OR1200_DU_DCR_SC]), match_cond4_ct[30:0]} >=
			 {(dvr4[31] ^ dcr4[`OR1200_DU_DCR_SC]), dvr4[30:0]});
		4'b1_110: match4 = 
			({(match_cond4_ct[31] ^ dcr4[`OR1200_DU_DCR_SC]), match_cond4_ct[30:0]} !=
			 {(dvr4[31] ^ dcr4[`OR1200_DU_DCR_SC]), dvr4[30:0]});
	endcase

//
// Watchpoint 4
//
always @(dmr1 or match4 or wp)
	case (dmr1[`OR1200_DU_DMR1_CW4])
		2'b00: wp[4] = match4;
		2'b01: wp[4] = match4 & wp[3];
		2'b10: wp[4] = match4 | wp[3];
		2'b11: wp[4] = 1'b0;
	endcase

//
// Compare To What (Match Condition 5)
//
always @(dcr5 or id_pc or dcpu_adr_i or dcpu_dat_dc
	or dcpu_dat_lsu or dcpu_we_i)
	case (dcr5[`OR1200_DU_DCR_CT])		// synopsys parallel_case
		3'b001:	match_cond5_ct = id_pc;		// insn fetch EA
		3'b010:	match_cond5_ct = dcpu_adr_i;	// load EA
		3'b011:	match_cond5_ct = dcpu_adr_i;	// store EA
		3'b100:	match_cond5_ct = dcpu_dat_dc;	// load data
		3'b101:	match_cond5_ct = dcpu_dat_lsu;	// store data
		3'b110:	match_cond5_ct = dcpu_adr_i;	// load/store EA
		default:match_cond5_ct = dcpu_we_i ? dcpu_dat_lsu : dcpu_dat_dc;
	endcase

//
// When To Compare (Match Condition 5)
//
always @(dcr5 or dcpu_cycstb_i)
	case (dcr5[`OR1200_DU_DCR_CT]) 		// synopsys parallel_case
		3'b000:	match_cond5_stb = 1'b0;		//comparison disabled
		3'b001:	match_cond5_stb = 1'b1;		// insn fetch EA
		default:match_cond5_stb = dcpu_cycstb_i; // any load/store
	endcase

//
// Match Condition 5
//
always @(match_cond5_stb or dcr5 or dvr5 or match_cond5_ct)
	casex ({match_cond5_stb, dcr5[`OR1200_DU_DCR_CC]})
		4'b0_xxx,
		4'b1_000,
		4'b1_111: match5 = 1'b0;
		4'b1_001: match5 =
			({(match_cond5_ct[31] ^ dcr5[`OR1200_DU_DCR_SC]), match_cond5_ct[30:0]} ==
			 {(dvr5[31] ^ dcr5[`OR1200_DU_DCR_SC]), dvr5[30:0]});
		4'b1_010: match5 = 
			({(match_cond5_ct[31] ^ dcr5[`OR1200_DU_DCR_SC]), match_cond5_ct[30:0]} <
			 {(dvr5[31] ^ dcr5[`OR1200_DU_DCR_SC]), dvr5[30:0]});
		4'b1_011: match5 = 
			({(match_cond5_ct[31] ^ dcr5[`OR1200_DU_DCR_SC]), match_cond5_ct[30:0]} <=
			 {(dvr5[31] ^ dcr5[`OR1200_DU_DCR_SC]), dvr5[30:0]});
		4'b1_100: match5 = 
			({(match_cond5_ct[31] ^ dcr5[`OR1200_DU_DCR_SC]), match_cond5_ct[30:0]} >
			 {(dvr5[31] ^ dcr5[`OR1200_DU_DCR_SC]), dvr5[30:0]});
		4'b1_101: match5 = 
			({(match_cond5_ct[31] ^ dcr5[`OR1200_DU_DCR_SC]), match_cond5_ct[30:0]} >=
			 {(dvr5[31] ^ dcr5[`OR1200_DU_DCR_SC]), dvr5[30:0]});
		4'b1_110: match5 = 
			({(match_cond5_ct[31] ^ dcr5[`OR1200_DU_DCR_SC]), match_cond5_ct[30:0]} !=
			 {(dvr5[31] ^ dcr5[`OR1200_DU_DCR_SC]), dvr5[30:0]});
	endcase

//
// Watchpoint 5
//
always @(dmr1 or match5 or wp)
	case (dmr1[`OR1200_DU_DMR1_CW5])
		2'b00: wp[5] = match5;
		2'b01: wp[5] = match5 & wp[4];
		2'b10: wp[5] = match5 | wp[4];
		2'b11: wp[5] = 1'b0;
	endcase

//
// Compare To What (Match Condition 6)
//
always @(dcr6 or id_pc or dcpu_adr_i or dcpu_dat_dc
	or dcpu_dat_lsu or dcpu_we_i)
	case (dcr6[`OR1200_DU_DCR_CT])		// synopsys parallel_case
		3'b001:	match_cond6_ct = id_pc;		// insn fetch EA
		3'b010:	match_cond6_ct = dcpu_adr_i;	// load EA
		3'b011:	match_cond6_ct = dcpu_adr_i;	// store EA
		3'b100:	match_cond6_ct = dcpu_dat_dc;	// load data
		3'b101:	match_cond6_ct = dcpu_dat_lsu;	// store data
		3'b110:	match_cond6_ct = dcpu_adr_i;	// load/store EA
		default:match_cond6_ct = dcpu_we_i ? dcpu_dat_lsu : dcpu_dat_dc;
	endcase

//
// When To Compare (Match Condition 6)
//
always @(dcr6 or dcpu_cycstb_i)
	case (dcr6[`OR1200_DU_DCR_CT]) 		// synopsys parallel_case
		3'b000:	match_cond6_stb = 1'b0;		//comparison disabled
		3'b001:	match_cond6_stb = 1'b1;		// insn fetch EA
		default:match_cond6_stb = dcpu_cycstb_i; // any load/store
	endcase

//
// Match Condition 6
//
always @(match_cond6_stb or dcr6 or dvr6 or match_cond6_ct)
	casex ({match_cond6_stb, dcr6[`OR1200_DU_DCR_CC]})
		4'b0_xxx,
		4'b1_000,
		4'b1_111: match6 = 1'b0;
		4'b1_001: match6 =
			({(match_cond6_ct[31] ^ dcr6[`OR1200_DU_DCR_SC]), match_cond6_ct[30:0]} ==
			 {(dvr6[31] ^ dcr6[`OR1200_DU_DCR_SC]), dvr6[30:0]});
		4'b1_010: match6 = 
			({(match_cond6_ct[31] ^ dcr6[`OR1200_DU_DCR_SC]), match_cond6_ct[30:0]} <
			 {(dvr6[31] ^ dcr6[`OR1200_DU_DCR_SC]), dvr6[30:0]});
		4'b1_011: match6 = 
			({(match_cond6_ct[31] ^ dcr6[`OR1200_DU_DCR_SC]), match_cond6_ct[30:0]} <=
			 {(dvr6[31] ^ dcr6[`OR1200_DU_DCR_SC]), dvr6[30:0]});
		4'b1_100: match6 = 
			({(match_cond6_ct[31] ^ dcr6[`OR1200_DU_DCR_SC]), match_cond6_ct[30:0]} >
			 {(dvr6[31] ^ dcr6[`OR1200_DU_DCR_SC]), dvr6[30:0]});
		4'b1_101: match6 = 
			({(match_cond6_ct[31] ^ dcr6[`OR1200_DU_DCR_SC]), match_cond6_ct[30:0]} >=
			 {(dvr6[31] ^ dcr6[`OR1200_DU_DCR_SC]), dvr6[30:0]});
		4'b1_110: match6 = 
			({(match_cond6_ct[31] ^ dcr6[`OR1200_DU_DCR_SC]), match_cond6_ct[30:0]} !=
			 {(dvr6[31] ^ dcr6[`OR1200_DU_DCR_SC]), dvr6[30:0]});
	endcase

//
// Watchpoint 6
//
always @(dmr1 or match6 or wp)
	case (dmr1[`OR1200_DU_DMR1_CW6])
		2'b00: wp[6] = match6;
		2'b01: wp[6] = match6 & wp[5];
		2'b10: wp[6] = match6 | wp[5];
		2'b11: wp[6] = 1'b0;
	endcase

//
// Compare To What (Match Condition 7)
//
always @(dcr7 or id_pc or dcpu_adr_i or dcpu_dat_dc
	or dcpu_dat_lsu or dcpu_we_i)
	case (dcr7[`OR1200_DU_DCR_CT])		// synopsys parallel_case
		3'b001:	match_cond7_ct = id_pc;		// insn fetch EA
		3'b010:	match_cond7_ct = dcpu_adr_i;	// load EA
		3'b011:	match_cond7_ct = dcpu_adr_i;	// store EA
		3'b100:	match_cond7_ct = dcpu_dat_dc;	// load data
		3'b101:	match_cond7_ct = dcpu_dat_lsu;	// store data
		3'b110:	match_cond7_ct = dcpu_adr_i;	// load/store EA
		default:match_cond7_ct = dcpu_we_i ? dcpu_dat_lsu : dcpu_dat_dc;
	endcase

//
// When To Compare (Match Condition 7)
//
always @(dcr7 or dcpu_cycstb_i)
	case (dcr7[`OR1200_DU_DCR_CT]) 		// synopsys parallel_case
		3'b000:	match_cond7_stb = 1'b0;		//comparison disabled
		3'b001:	match_cond7_stb = 1'b1;		// insn fetch EA
		default:match_cond7_stb = dcpu_cycstb_i; // any load/store
	endcase

//
// Match Condition 7
//
always @(match_cond7_stb or dcr7 or dvr7 or match_cond7_ct)
	casex ({match_cond7_stb, dcr7[`OR1200_DU_DCR_CC]})
		4'b0_xxx,
		4'b1_000,
		4'b1_111: match7 = 1'b0;
		4'b1_001: match7 =
			({(match_cond7_ct[31] ^ dcr7[`OR1200_DU_DCR_SC]), match_cond7_ct[30:0]} ==
			 {(dvr7[31] ^ dcr7[`OR1200_DU_DCR_SC]), dvr7[30:0]});
		4'b1_010: match7 = 
			({(match_cond7_ct[31] ^ dcr7[`OR1200_DU_DCR_SC]), match_cond7_ct[30:0]} <
			 {(dvr7[31] ^ dcr7[`OR1200_DU_DCR_SC]), dvr7[30:0]});
		4'b1_011: match7 = 
			({(match_cond7_ct[31] ^ dcr7[`OR1200_DU_DCR_SC]), match_cond7_ct[30:0]} <=
			 {(dvr7[31] ^ dcr7[`OR1200_DU_DCR_SC]), dvr7[30:0]});
		4'b1_100: match7 = 
			({(match_cond7_ct[31] ^ dcr7[`OR1200_DU_DCR_SC]), match_cond7_ct[30:0]} >
			 {(dvr7[31] ^ dcr7[`OR1200_DU_DCR_SC]), dvr7[30:0]});
		4'b1_101: match7 = 
			({(match_cond7_ct[31] ^ dcr7[`OR1200_DU_DCR_SC]), match_cond7_ct[30:0]} >=
			 {(dvr7[31] ^ dcr7[`OR1200_DU_DCR_SC]), dvr7[30:0]});
		4'b1_110: match7 = 
			({(match_cond7_ct[31] ^ dcr7[`OR1200_DU_DCR_SC]), match_cond7_ct[30:0]} !=
			 {(dvr7[31] ^ dcr7[`OR1200_DU_DCR_SC]), dvr7[30:0]});
	endcase

//
// Watchpoint 7
//
always @(dmr1 or match7 or wp)
	case (dmr1[`OR1200_DU_DMR1_CW7])
		2'b00: wp[7] = match7;
		2'b01: wp[7] = match7 & wp[6];
		2'b10: wp[7] = match7 | wp[6];
		2'b11: wp[7] = 1'b0;
	endcase

//
// Increment Watchpoint Counter 0
//
always @(wp or dmr2)
	if (dmr2[`OR1200_DU_DMR2_WCE0])
		incr_wpcntr0 = |(wp & ~dmr2[`OR1200_DU_DMR2_AWTC]);
	else
		incr_wpcntr0 = 1'b0;

//
// Match Condition Watchpoint Counter 0
//
always @(dwcr0)
	if (dwcr0[`OR1200_DU_DWCR_MATCH] == dwcr0[`OR1200_DU_DWCR_COUNT])
		wpcntr0_match = 1'b1;
	else
		wpcntr0_match = 1'b0;


//
// Watchpoint 8
//
always @(dmr1 or wpcntr0_match or wp)
	case (dmr1[`OR1200_DU_DMR1_CW8])
		2'b00: wp[8] = wpcntr0_match;
		2'b01: wp[8] = wpcntr0_match & wp[7];
		2'b10: wp[8] = wpcntr0_match | wp[7];
		2'b11: wp[8] = 1'b0;
	endcase


//
// Increment Watchpoint Counter 1
//
always @(wp or dmr2)
	if (dmr2[`OR1200_DU_DMR2_WCE1])
		incr_wpcntr1 = |(wp & dmr2[`OR1200_DU_DMR2_AWTC]);
	else
		incr_wpcntr1 = 1'b0;

//
// Match Condition Watchpoint Counter 1
//
always @(dwcr1)
	if (dwcr1[`OR1200_DU_DWCR_MATCH] == dwcr1[`OR1200_DU_DWCR_COUNT])
		wpcntr1_match = 1'b1;
	else
		wpcntr1_match = 1'b0;

//
// Watchpoint 9
//
always @(dmr1 or wpcntr1_match or wp)
	case (dmr1[`OR1200_DU_DMR1_CW9])
		2'b00: wp[9] = wpcntr1_match;
		2'b01: wp[9] = wpcntr1_match & wp[8];
		2'b10: wp[9] = wpcntr1_match | wp[8];
		2'b11: wp[9] = 1'b0;
	endcase

//
// Watchpoint 10
//
always @(dmr1 or dbg_ewt_i or wp)
	case (dmr1[`OR1200_DU_DMR1_CW10])
		2'b00: wp[10] = dbg_ewt_i;
		2'b01: wp[10] = dbg_ewt_i & wp[9];
		2'b10: wp[10] = dbg_ewt_i | wp[9];
		2'b11: wp[10] = 1'b0;
	endcase

`endif

//
// Watchpoints can cause trap exception
//
`ifdef OR1200_DU_HWBKPTS
assign du_hwbkpt = |(wp & dmr2[`OR1200_DU_DMR2_WGB]) | du_hwbkpt_hold | (dbg_bp_r & ~dsr[`OR1200_DU_DSR_TE]);
`else
assign du_hwbkpt = 1'b0;
`endif

// Hold du_hwbkpt if ex_freeze is active in order to cause trap exception 
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		du_hwbkpt_hold <=  1'b0;
	else if (du_hwbkpt & ex_freeze)
		du_hwbkpt_hold <=  1'b1;
	else if (!ex_freeze)
		du_hwbkpt_hold <=  1'b0;

`ifdef OR1200_DU_TB_IMPLEMENTED
//
// Simple trace buffer
// (right now hardcoded for Xilinx Virtex FPGAs)
//
// Stores last 256 instruction addresses, instruction
// machine words and ALU results
//

//
// Trace buffer write enable
//
assign tb_enw = ~ex_freeze & ~((ex_insn[31:26] == `OR1200_OR32_NOP) & ex_insn[16]);

//
// Trace buffer write address pointer
//
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		tb_wadr <=  8'h00;
	else if (tb_enw)
		tb_wadr <=  tb_wadr + 8'd1;

//
// Free running counter (time stamp)
//
always @(posedge clk or `OR1200_RST_EVENT rst)
	if (rst == `OR1200_RST_VALUE)
		tb_timstmp <=  32'h00000000;
	else if (!dbg_bp_r)
		tb_timstmp <=  tb_timstmp + 32'd1;

//
// Trace buffer RAMs
//

or1200_dpram_256x32 tbia_ram(
	.clk_a(clk),
	.rst_a(1'b0),
	.addr_a(spr_addr[7:0]),
	.ce_a(1'b1),
	.oe_a(1'b1),
	.do_a(tbia_dat_o),

	.clk_b(clk),
	.rst_b(1'b0),
	.addr_b(tb_wadr),
	.di_b(spr_dat_npc),
	.ce_b(1'b1),
	.we_b(tb_enw)

);

or1200_dpram_256x32 tbim_ram(
	.clk_a(clk),
	.rst_a(1'b0),
	.addr_a(spr_addr[7:0]),
	.ce_a(1'b1),
	.oe_a(1'b1),
	.do_a(tbim_dat_o),
	
	.clk_b(clk),
	.rst_b(1'b0),
	.addr_b(tb_wadr),
	.di_b(ex_insn),
	.ce_b(1'b1),
	.we_b(tb_enw)
);

or1200_dpram_256x32 tbar_ram(
	.clk_a(clk),
	.rst_a(1'b0),
	.addr_a(spr_addr[7:0]),
	.ce_a(1'b1),
	.oe_a(1'b1),
	.do_a(tbar_dat_o),
	
	.clk_b(clk),
	.rst_b(1'b0),
	.addr_b(tb_wadr),
	.di_b(rf_dataw),
	.ce_b(1'b1),
	.we_b(tb_enw)
);

or1200_dpram_256x32 tbts_ram(
	.clk_a(clk),
	.rst_a(1'b0),
	.addr_a(spr_addr[7:0]),
	.ce_a(1'b1),
	.oe_a(1'b1),
	.do_a(tbts_dat_o),

	.clk_b(clk),
	.rst_b(1'b0),
	.addr_b(tb_wadr),
	.di_b(tb_timstmp),
	.ce_b(1'b1),
	.we_b(tb_enw)
);

`else

assign tbia_dat_o = 32'h0000_0000;
assign tbim_dat_o = 32'h0000_0000;
assign tbar_dat_o = 32'h0000_0000;
assign tbts_dat_o = 32'h0000_0000;

`endif	// OR1200_DU_TB_IMPLEMENTED

`else	// OR1200_DU_IMPLEMENTED

//
// When DU is not implemented, drive all outputs as would when DU is disabled
//
assign dbg_bp_o = 1'b0;
assign du_dsr = {`OR1200_DU_DSR_WIDTH{1'b0}};
assign du_dmr1 = {25{1'b0}};
assign du_hwbkpt = 1'b0;

//
// Read DU registers
//
`ifdef OR1200_DU_READREGS
assign spr_dat_o = 32'h0000_0000;
`ifdef OR1200_DU_UNUSED_ZERO
`endif
`endif

`endif

endmodule
