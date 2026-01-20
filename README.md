# Oasis maps 
A 2-d map generator that creates a route from real U.S roads and intersections from two desired points. 

## Software Internals
Data strucutres used : Hashtables, Vectors, and Prioirty Queues

Graph : 

Intersection1 (PlaceB) -----Road---> Interseection2 (PlaceA)

Algorithm :
For the shortest path algorithm, I followed the UCS (uniform cost search) algorithm. The algorithm finds the shortest path by inserting and popping off a min heap using its total path length as a weight. The PathNodes keep track of all the different paths and how they connect. They allow the program to remember and track how different intersections connect in a path. The struct contains a pointer to its parent (the intersection prior), a road pointer that connects the current node with its paremt, and the total weight (length in miles) of the entire path. To keep track of the nodes visited, a new member was added to the Intersections struct, visited. This was set to true any time it was used in a path, preventing any loops or wasted paths from being created. 


Examples

MIA to ORL

<img width="797" height="785" alt="miamiToOrlando" src="https://github.com/user-attachments/assets/f5ba081a-659e-4d4a-84da-a87e27d6c092" />

MIA to LBBCK

<img width="742" height="772" alt="miamiToLubbock" src="https://github.com/user-attachments/assets/bdceaa4c-9d2a-4e6a-b35f-d3fbe7ce599f" />
