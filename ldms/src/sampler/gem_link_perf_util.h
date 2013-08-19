/*
 * Copyright (c) 2013 Open Grid Computing, Inc. All rights reserved.
 * Copyright (c) 2013 Sandia Corporation. All rights reserved.
 * Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
 * license for use of this work by or on behalf of the U.S. Government.
 * Export of this program may require a license from the United States
 * Government.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the BSD-type
 * license below:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *      Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *      Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *
 *      Neither the name of Sandia nor the names of any contributors may
 *      be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 *      Neither the name of Open Grid Computing nor the names of any
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *      Modified source versions must be plainly marked as such, and
 *      must not be misrepresented as being the original software.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * \file gem_link_perf_util.h
 * \brief Utilities for gem_link_perf/nic_perf_sampler also used in ncsa_unified
 */

#ifndef __GEM_LINK_PERF_UTIL_H_
#define __GEM_LINK_PERF_UTIL_H_

#define _GNU_SOURCE

#include <inttypes.h>
#include <unistd.h>
#include <sys/errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <rca_lib.h>
#include <rs_id.h>
#include <rs_meshcoord.h>
#include <gpcd_lib.h>
#include "gemini.h"
#include "ldms.h"
#include "ldmsd.h"

/* Array to hold counter names */
static char ns_gemlink_gemctrname[][32] = {
	"GEMINI_TCTR_VC0_INPUT_PHITS",
	"GEMINI_TCTR_VC1_INPUT_PHITS",
	"GEMINI_TCTR_VC0_INPUT_PACKETS",
	"GEMINI_TCTR_VC1_INPUT_PACKETS",
	"GEMINI_TCTR_INPUT_STALLS",
	"GEMINI_TCTR_OUTPUT_STALLS"
};

static char ns_gemlink_gemstatsname[][64] = {
	"SAMPLE_GEMINI_TCTR_LINK_BW",
	"SAMPLE_GEMINI_TCTR_LINK_USED_BW",
	"SAMPLE_GEMINI_TCTR_LINK_PACKETSIZE_AVE",
	"SAMPLE_GEMINI_TCTR_LINK_INPUT_STALLS",
	"SAMPLE_GEMINI_TCTR_LINK_OUTPUT_STALLS"
};

#define NETTOPODIM 3
static char nettopo_meshcoord_metricname[][64] = {
	"nettopo_mesh_coord_x",
	"nettopo_mesh_coord_y",
	"nettopo_mesh_coord_z"
};

/* Array to hold link direction */
static char ns_gemlink_gemctrdir[][4] = {
	"X+", "X-", "Y+", "Y-", "Z+", "Z-", "HH"
};

#define NUM_NIC_PERF_RAW 12

#define STR_WRAP(NAME) #NAME
#define PREFIX_ENUM_R(NAME) R_ ## NAME

#define NIC_PERF_RAW_LIST(WRAP) \
	WRAP(GM_ORB_PERF_VC1_STALLED),		\
		WRAP(GM_ORB_PERF_VC0_STALLED),	\
		WRAP(GM_ORB_PERF_VC1_PKTS),	\
		WRAP(GM_ORB_PERF_VC0_PKTS),	\
		WRAP(GM_ORB_PERF_VC1_FLITS),	\
		WRAP(GM_ORB_PERF_VC0_FLITS),	\
		WRAP(GM_NPT_PERF_NPT_FLIT_CNTR),	\
		WRAP(GM_NPT_PERF_NPT_PKT_CNTR),		\
		WRAP(GM_NPT_PERF_NPT_BLOCKED_CNTR),	\
		WRAP(GM_NPT_PERF_NPT_STALLED_CNTR),	\
		WRAP(GM_RAT_PERF_HEADER_FLITS_VC0),	\
		WRAP(GM_RAT_PERF_DATA_FLITS_VC0)

static char* nic_perf_raw_name[] = {
	NIC_PERF_RAW_LIST(STR_WRAP)
};

typedef enum {
	NIC_PERF_RAW_LIST(PREFIX_ENUM_R)
} nic_perf_raw_t;

int get_my_nid(void);
void get_my_coord(gemini_coord_t *coord);
void set_coord_invalid(gemini_coord_t *coord);
int coord_invalid(gemini_coord_t *coord);
int coord_valid(gemini_coord_t *coord);
int coords_equal(gemini_coord_t *a, gemini_coord_t *b);
int tid_to_tcoord(int tid, int *row, int *col);
int tcoord_to_tid(int row, int col, int *tid);
int str_to_tid(char *str);
int str_to_linkdir(char *str);
int str_to_linktype(char *str);
double tile_to_bw(ldmsd_msg_log_f* msglog_outer, int tile_type);
int get_my_pattern(ldmsd_msg_log_f* msglog_outer, int *pattern, int* zind);
int gem_link_perf_parse_interconnect_file(ldmsd_msg_log_f* msglog_outer,
					  char *filename,
					  gemini_coord_t *neighbor,
					  gemini_tile_t *tile,
					  gemini_coord_t *mycoord,
					  double (*max_link_bw)[],
					  int (*tiles_per_dir)[]);
gpcd_context_t *gem_link_perf_create_context(ldmsd_msg_log_f*);
gpcd_context_t *nic_perf_create_context(ldmsd_msg_log_f*);

#endif
