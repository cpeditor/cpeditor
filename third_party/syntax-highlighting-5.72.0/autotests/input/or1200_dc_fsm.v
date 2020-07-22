//////////////////////////////////////////////////////////////////////
////                                                              ////
////  OR1200's DC FSM                                             ////
////                                                              ////
////  This file is part of the OpenRISC 1200 project              ////
////  http://opencores.org/project,or1k                           ////
////                                                              ////
////  Description                                                 ////
////  Data cache state machine                                    ////
////                                                              ////
////  To Do:                                                      ////
////   - Test error during line read or write                     ////
////                                                              ////
////  Author(s):                                                  ////
////      - Damjan Lampret, lampret@opencores.org                 ////
////      - Julius Baxter, julius@opencores.org                   ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2000, 2010 Authors and OPENCORES.ORG           ////
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
// $Log: or1200_dc_fsm.v,v $
// Revision 2.0  2010/06/30 11:00:00  ORSoC
// Minor update: 
// Bugs fixed. 
//

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on
`include "or1200_defines.v"

`define OR1200_DCFSM_IDLE	3'd0
`define OR1200_DCFSM_CLOADSTORE	3'd1
`define OR1200_DCFSM_LOOP2	3'd2
`define OR1200_DCFSM_LOOP3	3'd3
`define OR1200_DCFSM_LOOP4	3'd4
`define OR1200_DCFSM_FLUSH5	3'd5
`define OR1200_DCFSM_INV6	3'd6
`define OR1200_DCFSM_WAITSPRCS7	3'd7



//
// Data cache FSM for cache line of 16 bytes (4x singleword)
//

module or1200_dc_fsm
  (
   // Clock and reset
   clk, rst,
   
   // Internal i/f to top level DC
   dc_en, dcqmem_cycstb_i, dcqmem_ci_i, dcqmem_we_i, dcqmem_sel_i,
   tagcomp_miss, biudata_valid, biudata_error, lsu_addr,
   dcram_we, biu_read, biu_write, biu_do_sel, dcram_di_sel, first_hit_ack, 
   first_miss_ack, first_miss_err, burst, tag_we, tag_valid, dc_addr, 
   dc_no_writethrough, tag_dirty, dirty, tag, tag_v, dc_block_flush, 
   dc_block_writeback, spr_dat_i, mtspr_dc_done, spr_cswe
   );

   //
   // I/O
   //
   input				clk;
   input				rst;
   input				dc_en;
   input				dcqmem_cycstb_i;
   input				dcqmem_ci_i;
   input				dcqmem_we_i;
   input [3:0] 				dcqmem_sel_i;
   input				tagcomp_miss;
   input				biudata_valid;
   input				biudata_error;
   input [31:0] 			lsu_addr;
   output [3:0] 			dcram_we;
   output				biu_read;
   output				biu_write;
   output 				dcram_di_sel;
   output 				biu_do_sel;
   output				first_hit_ack;
   output				first_miss_ack;
   output				first_miss_err;
   output				burst;
   output				tag_we;
   output 				tag_valid;
   output [31:0] 			dc_addr;
   input 				dc_no_writethrough;
   output 				tag_dirty;
   input 				dirty;
   input [`OR1200_DCTAG_W-2:0] 		tag;
   input 				tag_v;   
   input 				dc_block_flush;
   input 				dc_block_writeback;
   input [31:0] 			spr_dat_i;
   output 				mtspr_dc_done;
   input 				spr_cswe;
   
   
   //
   // Internal wires and regs
   //
   reg [31:0] 				addr_r;
   reg [2:0] 				state;
   reg [`OR1200_DCLS-1:0] 		cnt;
   reg 					hitmiss_eval;
   reg 					store;
   reg 					load;
   reg 					cache_inhibit;
   reg 					cache_miss;
   reg 					cache_dirty_needs_writeback;
   reg                                  did_early_load_ack;
   reg 					cache_spr_block_flush;
   reg 					cache_spr_block_writeback;
   reg 					cache_wb;   
   wire 				load_hit_ack;
   wire 				load_miss_ack;
   wire 				load_inhibit_ack;   
   wire 				store_hit_ack;
   wire 				store_hit_writethrough_ack;   
   wire 				store_miss_writethrough_ack;   
   wire 				store_inhibit_ack;
   wire 				store_miss_ack;
   wire 				dcram_we_after_line_load;
   wire 				dcram_we_during_line_load;
   wire 				tagram_we_end_of_loadstore_loop;
   wire 				tagram_dirty_bit_set;   
   wire 				writethrough;
   wire 				cache_inhibit_with_eval;
   wire [(`OR1200_DCLS-1)-2:0]		next_addr_word;

   //
   // Cache inhibit
   //
   
   // Indicates whether cache is inhibited, during hitmiss_eval and after
   assign cache_inhibit_with_eval = (hitmiss_eval & dcqmem_ci_i) |
				    (!hitmiss_eval & cache_inhibit);
   
   //
   // Generate of DCRAM write enables
   //

   // WE when non-writethrough, and had to wait for a line to load.
   assign dcram_we_after_line_load = (state == `OR1200_DCFSM_LOOP3) &
				    dcqmem_we_i & !cache_dirty_needs_writeback &
				     !did_early_load_ack;

   // WE when receiving the data cache line
   assign dcram_we_during_line_load = (state == `OR1200_DCFSM_LOOP2) & load & 
				      biudata_valid;   
   
   assign dcram_we =(// Write when hit - make sure it is only when hit - could
		     // maybe be doing write through and don't want to corrupt
		     // cache lines corresponding to the writethrough addr_r.
		     ({4{store_hit_ack | store_hit_writethrough_ack}} |
		     // Write after load of line
		     {4{dcram_we_after_line_load}}) & 
		     dcqmem_sel_i		 ) |
		    // Write during load
		    {4{dcram_we_during_line_load}};

   //
   // Tag RAM signals
   //
   
   // WE to tag RAM when we finish loading a line.
   assign tagram_we_end_of_loadstore_loop = ((state==`OR1200_DCFSM_LOOP2) & 
					     biudata_valid & !(|cnt));
   
`ifndef OR1200_DC_WRITETHROUGH
   // No writethrough, so mark a line dirty whenever we write to it
   assign tagram_dirty_bit_set = store_hit_ack | store_miss_ack;

   // Generate done signal for MTSPR instructions that may block execution
   assign mtspr_dc_done = // Either DC disabled or we're not selected, or
			  !dc_en | !spr_cswe |
			  // Requested address not valid or writeback and !dirty
			  ((state==`OR1200_DCFSM_FLUSH5) & 
			   (!tag_v | (cache_spr_block_writeback & !dirty))) |
			  // Writeback or flush is finished
			  ((state==`OR1200_DCFSM_LOOP3) & 
			   (cache_spr_block_flush | cache_spr_block_writeback))|
			  // Invalidate of clean line finished
			  ((state==`OR1200_DCFSM_INV6) & cache_spr_block_flush);
   
   
`else
 `ifdef OR1200_DC_NOSTACKWRITETHROUGH   
   // For dirty bit setting when having writethrough but not for stack
   assign tagram_dirty_bit_set = store_hit_ack | store_miss_ack;
 `else
   // Lines will never be dirty if always writethrough
   assign tagram_dirty_bit_set = 0;
 `endif
   
   assign mtspr_dc_done = 1'b1;
   
`endif

   assign tag_dirty = tagram_dirty_bit_set;
   
   // WE to tag RAM
   assign tag_we = tagram_we_end_of_loadstore_loop |		    
                    tagram_dirty_bit_set | (state == `OR1200_DCFSM_INV6);
   

   // Valid bit
   // Set valid when end of line load, or marking dirty (is still valid)
   assign tag_valid = ( tagram_we_end_of_loadstore_loop & 
			(load | (store & cache_spr_block_writeback)) ) |
		      tagram_dirty_bit_set;


   
   //
   // BIU read and write
   //

   assign biu_read = // Bus read request when:
		     // 1) Have a miss and not dirty or a load with inhibit
		     ((state == `OR1200_DCFSM_CLOADSTORE) &
		      (((hitmiss_eval & tagcomp_miss & !dirty & 
			 !(store & writethrough)) | 
			(load & cache_inhibit_with_eval)) & dcqmem_cycstb_i)) |
		     // 2) In the loop and loading
		     ((state == `OR1200_DCFSM_LOOP2) & load);
   

   assign biu_write = // Bus write request when:
		      // 1) Have a miss and dirty or store with inhibit
		      ((state == `OR1200_DCFSM_CLOADSTORE) & 
		       (((hitmiss_eval & tagcomp_miss & dirty) | 
			 (store & writethrough)) | 
			(store & cache_inhibit_with_eval)) & dcqmem_cycstb_i) |
		      // 2) In the loop and storing
		      ((state == `OR1200_DCFSM_LOOP2) & store);
   
   //
   // Select for data to actual cache RAM (from LSU or BIU)
   //
   // Data to DCRAM - from external bus when loading (from IU when store)
   assign dcram_di_sel = load;
   // Data to external bus - always from IU except in case of bursting back
   //                        the line to memory. (1 selects DCRAM)
   assign biu_do_sel = (state == `OR1200_DCFSM_LOOP2) & store;

   // 3-bit wire for calculating next word of burst write, depending on
   // line size of data cache.
   assign next_addr_word =  addr_r[`OR1200_DCLS-1:2] + 1;
   
   // Address to cache RAM (tag address also derived from this)   
   assign dc_addr =
		   // First check if we've got a block flush or WB op
		   ((dc_block_flush & !cache_spr_block_flush) | 
		   (dc_block_writeback & !cache_spr_block_writeback)) ? 
		   spr_dat_i :
		   (state==`OR1200_DCFSM_FLUSH5) ? addr_r:
		    // If no SPR action, then always put out address from LSU
		    (state==`OR1200_DCFSM_IDLE | hitmiss_eval) ? lsu_addr :
		    // Next, if in writeback loop, when ACKed must immediately
		    // output next word address (the RAM address takes a cycle
		    // to increment, but it's needed immediately for burst)
		    // otherwise, output our registered address.
		    (state==`OR1200_DCFSM_LOOP2 & biudata_valid & store ) ? 
		    {addr_r[31:`OR1200_DCLS], next_addr_word, 2'b00} : addr_r;
   
`ifdef OR1200_DC_WRITETHROUGH
 `ifdef OR1200_DC_NOSTACKWRITETHROUGH   
   assign writethrough = !dc_no_writethrough;
 `else
   assign writethrough = 1;
 `endif
`else
   assign writethrough = 0;
`endif
   
   //
   // ACK generation for LSU
   //
   
   // ACK for when it's a cache hit
   assign first_hit_ack = load_hit_ack | store_hit_ack | 
			  store_hit_writethrough_ack | 
			  store_miss_writethrough_ack |
			  store_inhibit_ack | store_miss_ack ;

   // ACK for when it's a cache miss - load only, is used in MUX for data back
   //                                  LSU straight off external data bus. In
   //                                  this was is also used for cache inhibit
   //                                  loads.
   // first_hit_ack takes precedence over first_miss_ack
   assign first_miss_ack = ~first_hit_ack & (load_miss_ack | load_inhibit_ack);
   
   // ACK cache hit on load
   assign load_hit_ack = (state == `OR1200_DCFSM_CLOADSTORE) & 
			 hitmiss_eval & !tagcomp_miss & !dcqmem_ci_i & load;
   
   // ACK cache hit on store, no writethrough
   assign store_hit_ack = (state == `OR1200_DCFSM_CLOADSTORE) & 
			  hitmiss_eval  & !tagcomp_miss & !dcqmem_ci_i &
			  store & !writethrough;
   
   // ACK cache hit on store with writethrough
   assign store_hit_writethrough_ack = (state == `OR1200_DCFSM_CLOADSTORE) & 
				       !cache_miss & !cache_inhibit &
				       store & writethrough & biudata_valid;
   
   // ACK cache miss on store with writethrough
   assign store_miss_writethrough_ack = (state == `OR1200_DCFSM_CLOADSTORE) & 
					cache_miss & !cache_inhibit &
					store & writethrough & biudata_valid;
      
   // ACK store when cacheinhibit
   assign store_inhibit_ack = (state == `OR1200_DCFSM_CLOADSTORE) &
			      store & cache_inhibit & biudata_valid;
   
   
   // Get the _early_ ack on first ACK back from wishbone during load only
   // Condition is that we're in the loop - that it's the first ack we get (can
   // tell from value of cnt), and we're loading a line to read from it (not
   // loading to write to it, in the case of a write without writethrough.)
   assign load_miss_ack =  ((state== `OR1200_DCFSM_LOOP2) & load &
			    (cnt==((1 << `OR1200_DCLS) - 4)) & biudata_valid & 
			    !(dcqmem_we_i & !writethrough));
   
   assign load_inhibit_ack = (state == `OR1200_DCFSM_CLOADSTORE) &
			     load & cache_inhibit & biudata_valid;   
   
   // This will be case of write through disabled, and had to load a line.
   assign store_miss_ack = dcram_we_after_line_load;
            
   assign first_miss_err = biudata_error & dcqmem_cycstb_i;

   // Signal burst when in the load/store loop. We will always try to burst.
   assign burst = (state == `OR1200_DCFSM_LOOP2);

   //
   // Main DC FSM
   //
   always @(posedge clk or `OR1200_RST_EVENT rst) begin
      if (rst == `OR1200_RST_VALUE) begin
	 state <=  `OR1200_DCFSM_IDLE;
	 addr_r <=  32'd0;
	 hitmiss_eval <=  1'b0;
	 store <=  1'b0;
	 load <=  1'b0;
	 cnt <=  `OR1200_DCLS'd0;
         cache_miss <=  1'b0;
	 cache_dirty_needs_writeback <= 1'b0;
	 cache_inhibit <=  1'b0;
	 did_early_load_ack <= 1'b0;
	 cache_spr_block_flush <= 1'b0;
	 cache_spr_block_writeback <= 1'b0;
      end
      else
	case (state)	// synopsys parallel_case
	  
          `OR1200_DCFSM_IDLE : begin
	     if (dc_en & (dc_block_flush | dc_block_writeback))
	       begin
		  cache_spr_block_flush <= dc_block_flush;
		  cache_spr_block_writeback <= dc_block_writeback;
		  hitmiss_eval <= 1'b1;
		  state <= `OR1200_DCFSM_FLUSH5;
		  addr_r <=  spr_dat_i;
	       end
	     else if (dc_en & dcqmem_cycstb_i)
	       begin
		  state <= `OR1200_DCFSM_CLOADSTORE;
		  hitmiss_eval <=  1'b1;
		  store <=  dcqmem_we_i;
		  load <=  !dcqmem_we_i;
	       end
	     
	     
          end // case: `OR1200_DCFSM_IDLE
	  
          `OR1200_DCFSM_CLOADSTORE: begin
	     hitmiss_eval <=  1'b0;
	     if (hitmiss_eval) begin
                cache_inhibit <=  dcqmem_ci_i; // Check for cache inhibit here
                cache_miss <=  tagcomp_miss;
		cache_dirty_needs_writeback <= dirty;
		addr_r <=  lsu_addr;
             end

	     // Evaluate any cache line load/stores in first cycle:
			     //
	     if (hitmiss_eval & tagcomp_miss & !(store & writethrough) &
		 !dcqmem_ci_i)
	       begin
		  // Miss - first either:
		  //  1) write back dirty line 
		  if (dirty) begin
		     // Address for writeback
		     addr_r <=  {tag, lsu_addr[`OR1200_DCINDXH:2],2'd0};
		     load <= 1'b0;
		     store <= 1'b1;
`ifdef OR1200_VERBOSE		     
		     $display("%t: dcache miss and dirty", $time);
`endif
		  end
		  //  2) load requested line
		  else begin
		     addr_r <=  lsu_addr;
		     load <= 1'b1;
		     store <= 1'b0;
		  end // else: !if(dirty)
		  state <= `OR1200_DCFSM_LOOP2;		  
		  // Set the counter for the burst accesses
		  cnt <=  ((1 << `OR1200_DCLS) - 4);
	       end
             else if (// Strobe goes low
		      !dcqmem_cycstb_i |
		      // Cycle finishes
		      (!hitmiss_eval & (biudata_valid | biudata_error)) |
		      // Cache hit in first cycle....
		      (hitmiss_eval & !tagcomp_miss & !dcqmem_ci_i &
		      // .. and you're not doing a writethrough store..
		      !(store & writethrough))) begin
                state <=  `OR1200_DCFSM_IDLE;
                load <=  1'b0;
		store <= 1'b0;
		cache_inhibit <= 1'b0;
		cache_dirty_needs_writeback <= 1'b0;
             end	     
          end // case: `OR1200_DCFSM_CLOADSTORE	  
	  
          `OR1200_DCFSM_LOOP2 : begin // loop/abort	     
             if (!dc_en| biudata_error) begin
                state <=  `OR1200_DCFSM_IDLE;
                load <=  1'b0;
		store <= 1'b0;
		cnt <= `OR1200_DCLS'd0;
             end
             if (biudata_valid & (|cnt)) begin
                cnt <=  cnt - 4;
                addr_r[`OR1200_DCLS-1:2] <=  addr_r[`OR1200_DCLS-1:2] + 1;
             end
	     else if (biudata_valid & !(|cnt)) begin
		state <= `OR1200_DCFSM_LOOP3;
		addr_r <=  lsu_addr;
		load <= 1'b0;
		store <= 1'b0;
	     end

	     // Track if we did an early ack during a load
	     if (load_miss_ack)
	       did_early_load_ack <= 1'b1;
	     

          end // case: `OR1200_DCFSM_LOOP2
	  
	  `OR1200_DCFSM_LOOP3: begin // figure out next step
	     if (cache_dirty_needs_writeback) begin
		// Just did store of the dirty line so now load new one
		load <= 1'b1;
		// Set the counter for the burst accesses
		cnt <=  ((1 << `OR1200_DCLS) - 4);
		// Address of line to be loaded
		addr_r <=  lsu_addr;
		cache_dirty_needs_writeback <= 1'b0;
		state <= `OR1200_DCFSM_LOOP2;
	     end // if (cache_dirty_needs_writeback)
	     else if (cache_spr_block_flush | cache_spr_block_writeback) begin
		// Just wrote back the line to memory, we're finished.
		cache_spr_block_flush <= 1'b0;
		cache_spr_block_writeback <= 1'b0;
		state <= `OR1200_DCFSM_WAITSPRCS7;
	     end
	     else begin
		// Just loaded a new line, finish up
		did_early_load_ack <= 1'b0;
		state <= `OR1200_DCFSM_LOOP4;
	     end
	  end // case: `OR1200_DCFSM_LOOP3

	  `OR1200_DCFSM_LOOP4: begin
	     state <=  `OR1200_DCFSM_IDLE;
	  end

	  `OR1200_DCFSM_FLUSH5: begin
	     hitmiss_eval <= 1'b0;
	     if (hitmiss_eval & !tag_v)
	       begin
		  // Not even cached, just ignore
		  cache_spr_block_flush <= 1'b0;
		  cache_spr_block_writeback <= 1'b0;
		  state <=  `OR1200_DCFSM_WAITSPRCS7;
	       end
	     else if (hitmiss_eval & tag_v)
	       begin
		  // Tag is valid - what do we do?
		  if ((cache_spr_block_flush | cache_spr_block_writeback) & 
		      dirty) begin
		     // Need to writeback
		     // Address for writeback (spr_dat_i has already changed so
		     // use line number from addr_r)
		     addr_r <=  {tag, addr_r[`OR1200_DCINDXH:2],2'd0};
		     load <= 1'b0;
		     store <= 1'b1;
`ifdef OR1200_VERBOSE		     
		     $display("%t: block flush: dirty block", $time);
`endif
		     state <= `OR1200_DCFSM_LOOP2;		  
		     // Set the counter for the burst accesses
		     cnt <=  ((1 << `OR1200_DCLS) - 4);
		  end
		  else if (cache_spr_block_flush & !dirty)
		    begin
		       // Line not dirty, just need to invalidate
		       state <=  `OR1200_DCFSM_INV6;
		    end // else: !if(dirty)
		  else if (cache_spr_block_writeback & !dirty)
		    begin
		       // Nothing to do - line is valid but not dirty
		       cache_spr_block_writeback <= 1'b0;
		       state <=  `OR1200_DCFSM_WAITSPRCS7;
		    end
	  end // if (hitmiss_eval & tag_v)
	  end
	  `OR1200_DCFSM_INV6: begin
	     cache_spr_block_flush <= 1'b0;
	     // Wait until SPR CS goes low before going back to idle
	     if (!spr_cswe)
	       state <=  `OR1200_DCFSM_IDLE;
	  end
	  `OR1200_DCFSM_WAITSPRCS7: begin
	     // Wait until SPR CS goes low before going back to idle
	     if (!spr_cswe)
	       state <=  `OR1200_DCFSM_IDLE;
	  end

	endcase // case (state)
      
   end // always @ (posedge clk or `OR1200_RST_EVENT rst)
   

endmodule
