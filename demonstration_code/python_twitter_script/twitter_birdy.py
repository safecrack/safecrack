from birdy.twitter import *
from jsonobject import *
import re
import time
import random

con_key = 'twitter consumer key'
con_sec = 'twitter consumer secret'
client = AppClient(con_key, con_sec)
access_token = client.get_access_token()
resource = client.api.search.tweets
response = resource.get(q='#twitterhashtag')

def get_random():
    first_num = random.randrange(1,99,1)
    if len(str(first_num)) == 1:
        first_num = "%02d" % (first_num,)
    second_num = random.randrange(1,99,1)
    if len(str(second_num)) == 1:
        second_num = "%02d" % (second_num,)
    third_num = random.randrange(1,99,1)
    if len(str(third_num)) == 1:
        third_num = "%02d" % (third_num,)
    final_num = "%s %s %s" % (first_num, second_num, third_num)
    return final_num 


def get_tweets():
    tweet_list = []
    response = resource.get(q='#twitterhashtag')
    for i in response.data.statuses:
        tweet = i.text
        tweet = re.findall('[0-9]{2} [0-9]{2} [0-9]{2}', tweet)
        if len(tweet) > 0:
            tweet = tweet[0]
            tweet = json.dumps(tweet)
            tweet = tweet.strip('"')
            tweet_list.append(tweet)
    return tweet_list

def gimme_tweet(old, new):
    old = list(new)
    new = []
    
    new = get_tweets()
    if len(new) > len(old):
        try_tweet = list(set(new) - set(old))
        print "Trying %s combination(s) from Twitter!" % len(try_tweet)
        return try_tweet, new, old
    else:
        print "Generated random combination"
        return [get_random()], new, old
