from collections import defaultdict
from sortedcontainers import SortedDict
from datetime import datetime, timedelta


class MessageDispatcher:
    def __init__(self) -> None:
        # maps time to list of message/recipient pairs
        self.queue = SortedDict()
        self.listeners = defaultdict(dict)
        self.received = defaultdict(list)

    def update(self):
        while len(self.queue) > 0 and self.queue.peekitem(index=0)[0] <= datetime.now():
            # print(self.queue.peekitem(index=0)[0])
            messages = self.queue.popitem(index=0)[1]
            for receiver, tag in messages:
                receiver.handleMessage(tag)

    def dispatchMessage(self, tag):
        now = datetime.now()
        self.received[tag].append(now)
        for listener, delay in self.listeners[tag].items():
            if delay == 0:
                listener.handleMessage(tag)
            else:
                time = now + timedelta(microseconds=delay)

                if time in self.queue:
                    self.queue[time].append((listener, tag))
                else:
                    self.queue[time] = [(listener, tag)]

    def addListener(self, listener, tag, delay=0):
        self.listeners[tag][listener] = delay

    def removeListener(self, listener, tag):
        # remove listener if present
        self.listeners[tag].pop(listener, None)


class Telegraph:
    def __init__(self, id) -> None:
        self.id = id
        self.received = defaultdict(list)

    def handleMessage(self, tag):
        self.received[tag].append(datetime.now())
