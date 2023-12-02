import multiprocessing

queue = multiprocessing.Queue()

queue.put("asd")

print(queue.get())
