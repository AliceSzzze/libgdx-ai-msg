from collections import defaultdict, deque
from sortedcontainers import SortedDict
from datetime import datetime, timedelta


class MessageDispatcher:
    def __init__(self) -> None:
        # maps time to list of message/recipient pairs
        self.mailboxes = {}
        self.received = defaultdict(list)

    def update(self):
        for mailbox in self.mailboxes.values():
            mailbox.update()
            
    def addMailbox(self, tag):
        self.mailboxes[tag] = Mailbox(tag)
        
    def removeMailbox(self, tag):
        del self.mailboxes[tag]

    def dispatchMessage(self, tag):
        self.received[tag].append(datetime.now())
        self.mailboxes[tag].dispatch()
        
    def addListener(self, listener, tag, delay=0):
        self.mailboxes[tag].addListener(listener, delay)

    def removeListener(self, listener, tag):
        # remove listener if present
        self.mailboxes[tag].removeListener(listener)

class Mailbox:
    def __init__(self, tag) -> None:
        self.tag = tag
        self.listeners = defaultdict(float)
        self.delays = SortedDict()
        self.immediate = set()
        self.messages = deque()
        
    def addListener(self, listener, delay=0.0):
        self.listeners[listener] = delay
        if delay == 0.0:
            self.immediate.add(listener)
        else:
            if delay in self.delays:
                self.delays[delay].add(listener)
            else:
                self.delays[delay] = set([listener])
                

    def removeListener(self, listener):
        # remove listener if present
        if listener in self.listeners:
            d = self.listeners[listener]
            if d == 0.0:
                self.immediate.remove(listener)
            else:
                self.delays[d].remove(listener)
                if len(self.delays[d]) == 0:
                    del self.delays[d]
            del self.listeners[listener]
        
    def dispatch(self):
        for imm in self.immediate:
            imm.handleMessage(self.tag)
        
        if len(self.delays) > 0:
            self.messages.append([datetime.now(), 0])

    def update(self):
        pops = 0
        for msg in self.messages:
            maxTime = (datetime.now()-msg[0])/ timedelta(microseconds=1)
            recRange = self.delays.irange(msg[1], maxTime, inclusive = (False, True))
            
            for rec in recRange:
                for receiver in self.delays[rec]:
                    receiver.handleMessage(self.tag)
                    
            if self.delays.bisect_right(maxTime) == len(self.delays):
                pops += 1
                
            msg[1] = maxTime
        
        while pops > 0:
            self.messages.popleft()
            pops -= 1


class Telegraph:
    def __init__(self, id) -> None:
        self.id = id
        self.received = defaultdict(list)

    def handleMessage(self, tag):
        self.received[tag].append(datetime.now())
