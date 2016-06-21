#!/usr/bin/env python

import sys
import serial
import re
import twitter_birdy

new_tweet_list = []
old_tweet_list = []
try_tweet = []
found = 0

print "Connecting to arduino..."
ser = serial.Serial('/dev/ttyACM3', 115200)
print "Connected!"


while found == 0:
  print "---------------------------------"
  print "Waiting for arduino"
  ser_line = ser.readline()
  sys.stdout.write("arduino:" + ser_line)
  if ser_line != "ready\n":
    continue
  print "Waiting for combination..."

  try_tweet, new_tweet_list, old_tweet_list = twitter_birdy.gimme_tweet(old_tweet_list, new_tweet_list)
  for combo in try_tweet:
    combo = combo + "\n"
    if not combo:
      print "End found, exiting loop"
      break
    regex = re.compile('^[0-9]{2} [0-9]{2} [0-9]{2}$')
    if regex.match(combo):
      print "Combo %s is good, going to send to arduino" % combo.rstrip()
      ser.write(combo)
      i = 0
      while i <= 2:
          i += 1
          result = ser.readline()
          if result == "wrong\n":
              break
      if result == "ready\n":
          print "Your counting is wrong"
      elif result == "nothing\n":
          print "Found Combo!...Stopping"
          found = 1
          break
      elif result == "wrong\n":
          print "Hall effect triggered, moving on"
      print "---------------------------------"
    else:
      print "Invalid combination received:"
