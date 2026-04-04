#include "scanner.h"

#include "adc.h"
#include "frame.h"
#include "mux.h"
#include "timer.h"

#include <stdlib.h>

// TODO refactor, build proper state machine.

static enum {
  PENDING,
  SCANNING,
  ADVANCING,
} _state = PENDING;

static size_t ctr = 0;
static size_t row = 0;
static size_t col = 0;

static void _tick() {
  switch (_state) {
  case PENDING:
    if (!ctr) {
      _state = SCANNING;
    }
    break;
  case SCANNING:
    _state = ADVANCING;
    adc_read(frame + row * WIDTH + col);
    break;
  case ADVANCING:
    _state = SCANNING;
    row += 1;
    row %= HEIGHT;
    adc_select(row);
    if (!row) {
      col += 1;
      col %= WIDTH;
      mux_select(col);
      if (!col) {
        _state = PENDING;
        frame_ready = true;
      }
    }
    break;
  }
  ctr += 1;
  ctr %= 100;
}

void scanner_init() { timer_handler = _tick; }
