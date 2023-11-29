import mailboxes
import msg
from random import randint
import time
from collections import deque
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from datetime import  timedelta
from scipy import stats


def seperateMessages(messages):
    dispatcher = msg.MessageDispatcher()
    t1 = msg.Telegraph(1)
    t2 = msg.Telegraph(2)
    t3 = msg.Telegraph(3)
    t4 = msg.Telegraph(4)
    
    dispatcher.addListener(t1, 1, 100)
    dispatcher.addListener(t3, 1, 200)
    dispatcher.addListener(t4, 1, 0)
    
    dispatcher.addListener(t2, 2, 2000)
    dispatcher.addListener(t3, 2, 1500)
    dispatcher.addListener(t4, 2, 0)
    
    duration = 5.0
    start = time.time()
        
    while time.time() - start < duration+3:
        dispatcher.update()
        
        while len(messages) > 0 and time.time() - start >= messages[0][0]:
            dispatcher.dispatchMessage(messages.popleft()[1])
            
    td = timedelta(microseconds=1)
    d = {"timestamp": [], "delays (ms)": []}
    for idx, recTime in enumerate(dispatcher.received[1]):
        d["timestamp"].append(t1.received[1][idx])
        d["delays (ms)"].append((t1.received[1][idx] - recTime)/td - 100)
        d["timestamp"].append(t3.received[1][idx])
        d["delays (ms)"].append((t3.received[1][idx] - recTime)/td - 200)
        d["timestamp"].append(t4.received[1][idx])
        d["delays (ms)"].append((t4.received[1][idx] - recTime)/td )
        
    for idx, recTime in enumerate(dispatcher.received[2]):
        d["timestamp"].append(t2.received[2][idx])
        d["delays (ms)"].append((t2.received[2][idx] - recTime)/td - 2000) 
        d["timestamp"].append(t3.received[2][idx])
        d["delays (ms)"].append((t3.received[2][idx] - recTime)/td - 1500) 
        d["timestamp"].append(t4.received[2][idx])
        d["delays (ms)"].append((t4.received[2][idx] - recTime)/td )
    
    df = pd.DataFrame.from_dict(d)
    df.set_index('timestamp')
    df.sort_index()
    df[np.abs(stats.zscore(df['delays (ms)'])) < 3]
    # df[(np.abs(stats.zscore(df['delays (ms)'])) < 3).all(axis=1)]
    plt.figure(figsize=(8, 4))
    plt.plot(df.index, df['delays (ms)'], marker='o', linestyle='-')
    plt.title('Seperate messgae implementation delay')
    plt.xlabel('Timestamp')
    plt.ylabel('Delay (ms)')
    plt.grid(True)

    # Save the plot to a file (e.g., as a PNG image)
    plt.savefig('msg3.png')

def mailbox(messages):
    dispatcher = mailboxes.MessageDispatcher()
    t1 = mailboxes.Telegraph(1)
    t2 = mailboxes.Telegraph(2)
    t3 = mailboxes.Telegraph(3)
    t4 = mailboxes.Telegraph(4)
    
    dispatcher.addMailbox(1)
    dispatcher.addMailbox(2)
    
    dispatcher.addListener(t1, 1, 100)
    dispatcher.addListener(t3, 1, 200)
    dispatcher.addListener(t4, 1, 0)
    
    dispatcher.addListener(t2, 2, 2000)
    dispatcher.addListener(t3, 2, 1500)
    dispatcher.addListener(t4, 2, 0)
    
    duration = 5.0
    start = time.time()
    
    while time.time() - start < duration+3:
        dispatcher.update()
        
        while len(messages) > 0 and time.time() - start >= messages[0][0]:
            dispatcher.dispatchMessage(messages.popleft()[1])
            
    td = timedelta(microseconds=1)
    d = {"timestamp": [], "delays (ms)": []}
    for idx, recTime in enumerate(dispatcher.received[1]):
        d["timestamp"].append(t1.received[1][idx])
        d["delays (ms)"].append((t1.received[1][idx] - recTime)/td - 100)
        d["timestamp"].append(t3.received[1][idx])
        d["delays (ms)"].append((t3.received[1][idx] - recTime)/td - 200)
        d["timestamp"].append(t4.received[1][idx])
        d["delays (ms)"].append((t4.received[1][idx] - recTime)/td )
        
    for idx, recTime in enumerate(dispatcher.received[2]):
        d["timestamp"].append(t2.received[2][idx])
        d["delays (ms)"].append((t2.received[2][idx] - recTime)/td - 2000) 
        d["timestamp"].append(t3.received[2][idx])
        d["delays (ms)"].append((t3.received[2][idx] - recTime)/td - 1500) 
        d["timestamp"].append(t4.received[2][idx])
        d["delays (ms)"].append((t4.received[2][idx] - recTime)/td )
    
    df = pd.DataFrame.from_dict(d)
    df.set_index('timestamp')
    df.sort_index()
    df[np.abs(stats.zscore(df['delays (ms)'])) < 3]
    # df[(np.abs(stats.zscore(df['delays (ms)'])) < 3).all(axis=1)]
    plt.figure(figsize=(8, 4))
    plt.plot(df.index, df['delays (ms)'], marker='o', linestyle='-')
    plt.title('Mailbox implementation delay')
    plt.xlabel('Timestamp')
    plt.ylabel('Delay (ms)')
    plt.grid(True)

    # Save the plot to a file (e.g., as a PNG image)
    plt.savefig('mailbox3.png')
    
def main():
    messages = deque()
    duration = 5.0
    i = 0.0
    while i < duration:
        messages.append((i, randint(1, 2)))
        i += 0.05
    seperateMessages(deque(messages))
    mailbox(deque(messages))

if __name__ == "__main__":
    main()