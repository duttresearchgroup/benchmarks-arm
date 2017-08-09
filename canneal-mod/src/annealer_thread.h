// annealer_thread.h
//
// Created by Daniel Schwartz-Narbonne on 14/04/07.
// Modified by Christian Bienia
//
// Copyright 2007-2008 Princeton University
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.


#ifndef ANNEALER_THREAD_H
#define ANNEALER_THREAD_H

#ifdef ENABLE_THREADS
#include <pthread.h>
#endif

#include <assert.h>

#include "annealer_types.h"
#include "netlist.h"
#include "netlist_elem.h"
#include "rng.h"

#include <runtime/uapi/beats.h>

class annealer_thread 
{
public:
	enum move_decision_t{
		move_decision_accepted_good,
		move_decision_accepted_bad,
		move_decision_rejected
	};

	annealer_thread(
		netlist* netlist,
		int nthreads,
		int swaps_per_temp,
		int start_temp,
		int number_temp_steps
	)
	:_netlist(netlist),
	_keep_going_global_flag(true),
	_moves_per_thread_temp(swaps_per_temp/nthreads),
	_start_temp(start_temp),
	_number_temp_steps(number_temp_steps),
	beats_en(false),
	_vitamins_info(0),
	_vitamins_beats(0)
	{
		assert(_netlist != NULL);
#ifdef ENABLE_THREADS
		pthread_barrier_init(&_barrier, NULL, nthreads);
#endif
	};
	
	~annealer_thread() {
#ifdef ENABLE_THREADS
		pthread_barrier_destroy(&_barrier);
#endif
	}					
	void Run();

	void SetBeats(task_beat_info_t *info, task_beat_data_t *data)	{ beats_en = true; _vitamins_info = info; _vitamins_beats = data; };

					
protected:
	move_decision_t accept_move(routing_cost_t delta_cost, double T, Rng* rng);
	routing_cost_t calculate_delta_routing_cost(netlist_elem* a, netlist_elem* b);
	bool keep_going(int temp_steps_completed, int accepted_good_moves, int accepted_bad_moves);

protected:
	netlist* _netlist;		
	bool _keep_going_global_flag;
	int _moves_per_thread_temp;
	int _start_temp;
	int _number_temp_steps;

	bool beats_en;  //if heartbeats are enabled
	task_beat_info_t *_vitamins_info;
	task_beat_data_t *_vitamins_beats;

#ifdef ENABLE_THREADS
	pthread_barrier_t _barrier;
#endif
};

#endif

