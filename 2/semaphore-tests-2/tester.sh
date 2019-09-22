#!/bin/bash

echo "[i] Make files..."

make all

status="$?"

if [[ ! "$status" = "0" ]]; then
  echo "[!] Make failed :("
  exit 1
fi

echo "[@1] Run TEST SIMPLE..."

timeout 2s ./build/simple
status="$?"

if [[ "$status" = "124" ]]; then
  echo "[!] SIMPLE TIMED OUT!"
  echo "[i] There may be a problem with dead lock..."
  echo "[i] Please test again or trash your solution"
  echo "[?] Anyway have a nice day"
fi

if [[ ! "$status" = "0" ]]; then
        echo "[!] Tester failed :("
        exit 1
fi

echo "[@2] Run TEST HANGING..."

timeout 3s ./build/hanging
status="$?"

if [[ ! "$status" = "124" ]]; then
  echo "[!] ERROR"
  echo "[!] Hanging test does not HANG it is a problem with your solution"
  echo "[i] Haning condition is not valid."
  echo "[i] Exitting..."
  exit 1
fi

echo "[@3] Run TEST TWO..."

timeout 12s ./build/two
status="$?"

if [[ "$status" = "123" ]]; then
  echo "[!] TWO test is HANGING. Probable reason is dead-lock :C"
  exit 1
fi

if [[ ! "$status" = "0" ]]; then
  echo "[!] TWO test is not passing."
  echo "[!] Your wait conditions ARE BROKEN."
  echo "[i] Please fix the error."
  exit 1
fi

echo "[@3] Run TEST MULTI..."

timeout 120s ./build/multi
status="$?"

if [[ "$status" = "123" ]]; then
  echo "[!] MULTI test is HANGING. Probable reason is dead-lock :C"
  exit 1
fi

if [[ ! "$status" = "0" ]]; then
  echo "[!] MULTI test is working TOO LONG."
  echo "[!] Your wait conditions ARE BROKEN maybe?"
  echo "[i] Please fix the error."
  exit 1
fi

echo "[@3] Run TEST DAFUQ.."

timeout 40s ./build/dafuq
status="$?"

if [[ "$status" = "123" ]]; then
  echo "[!] DAFUQ test is HANGING. Probable reason is dead-lock :C"
  exit 1
fi

if [[ ! "$status" = "0" ]]; then
  echo "[!] DAFUQ test is working TOO LONG."
  echo "[!] Your wait conditions ARE BROKEN maybe?"
  echo "[i] Please fix the error."
  exit 1
fi

echo "[@3] Run TEST PROBEREN_TIME.."

timeout 7s ./build/proberen_time_test
status="$?"

if [[ "$status" = "123" ]]; then
  echo "[!] PROBEREN_TIME test is HANGING. Probable reason is dead-lock :C"
  exit 1
fi

if [[ ! "$status" = "0" ]]; then
  echo "[!] PROBEREN_TIME test is working TOO LONG."
  echo "[!] Your wait conditions ARE BROKEN maybe?"
  echo "[i] Please fix the error."
  exit 1
fi

echo "[i] Everythin seems fine. All test are passing :D"
echo "[i] OK :D"

