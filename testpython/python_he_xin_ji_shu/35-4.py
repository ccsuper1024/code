#orderboot爬虫
import copy
import json
import ssl
import time
import websocket

class OrderBook(object):
    BIDS='bids'
    ASKS='ask'

    def __init__(self, limit=20):
        self.limit = limit

        #(price, amount)
        self.bids = {}      #字典
        self.asks = {}
        self.bids_sorted = []
        self.asks_sorted = []

    def insert(self, price, amount, direction):
        if direction == self.BIDS:
            if amount == 0:
                if price in self.bids:
                    del self.bids[price]        #del关键字
                else:
                    self.bids[price] = amount
        elif direction == self.ASKS:
            if amount == 0:
                if price in self.asks:
                    del self.asks[price]
        else:
            print('WARNING:unknown direction{}'.format(direction))

    def sort_and_truncate(self):
        #sort 


