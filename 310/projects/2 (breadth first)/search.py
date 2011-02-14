#!/usr/bin/env python
# Breadth first search prototype
#
# Shaun Meyer, Feb 2011
#

matrix = [
    [1, 1, 1, 0, 0],
    [1, 1, 1, 0, 0],
    [1, 1, 1, 0, 1],
    [0, 0, 0, 1, 1],
    [0, 0, 1, 1, 1]]

start_node = 0
end_node = 3

q = []

def enqueue(item):
    q.insert(0, item)

def dequeue():
    return q.pop()

def front():
    return q[-1]

enqueue(start_node)
current_node = front()
from_list = [-1,-1,-1,-1,-1]
visited = [0,0,0,0,0]

while (current_node != end_node):
    print q
    current_node = dequeue()
    for edge in xrange(len(matrix[current_node])):

        if (matrix[current_node][edge] == 0):
            continue;
        if (visited[edge]):
            continue;

        print "Node: %d,%d" %( current_node, edge), 
        from_list[edge] = current_node
        visited[edge] = True
        enqueue(edge)
    print ""

print "visited: ", visited
print "from_list: "
for i in xrange(len(from_list)):
    print "%d: %d" % (i, from_list[i])

def print_path(end, start):
    i = end_node
    ret = [i]
    while (i != start):
        ret.append(from_list[i])
        i = from_list[i]
    return ret

print print_path(end_node, start_node)
    

    
