import sys
from math import sqrt

class Street_Graph():

    def __init__(self,streets):
        self.street_segments=Street_Graph.set_street_segments(streets)
        self.lines=[]
        self.vertices=set()
        self.intersections_points=[]
        self.intersection_segments={}
        self.edges=[]
        self.add_lines()

    @staticmethod
    def set_street_segments(streets):
        for street_name, street_coordinates in streets.items():
            length=len(street_coordinates)
            for i in range(length):
                if(isinstance(street_coordinates[0],tuple)):
                    break
                
                coordinates=street_coordinates[0].split(',')
                x = float(coordinates[0][1:])
                y = float(coordinates[1][:len(coordinates[1])-1])
                
                streets[street_name].remove(street_coordinates[0])
                streets[street_name].append((x,y))
        return streets

        
    def add_lines(self):
        for k, (street_name, street_coordinates) in enumerate(self.street_segments.items()):
            for i in range(len(street_coordinates) - 1):
                start_point = (street_coordinates[i][0], street_coordinates[i][1])
                end_point = (street_coordinates[i + 1][0], street_coordinates[i + 1][1])
                self.lines.append([street_name,[start_point, end_point]])
        
    
    @staticmethod
    def find_intersection(line_segment_1, line_segment_2):
        x1, y1 = line_segment_1[0]
        x2, y2 = line_segment_1[1]
        x3, y3 = line_segment_2[0]
        x4, y4 = line_segment_2[1]

        # Calculate the slopes of the two lines
        m1 = (y2 - y1) / (x2 - x1) if (x2 - x1) != 0 else float('inf')
        m2 = (y4 - y3) / (x4 - x3) if (x4 - x3) != 0 else float('inf')

        # Check if the lines are parallel (slopes are equal)
        if m1 == m2:
            return None  # Lines are parallel, no intersection

        # Calculate the intersection point
        if m1 == float('inf'):
            # Line 1 is vertical, so x1 is the x-coordinate of the intersection
            x_int = x1
            y_int = m2 * (x1 - x3) + y3
        elif m2 == float('inf'):
            # Line 2 is vertical, so x3 is the x-coordinate of the intersection
            x_int = x3
            y_int = m1 * (x3 - x1) + y1
        else:
            # Calculate the intersection point using the equation of lines
            x_int = (y3 - y1 + m1 * x1 - m2 * x3) / (m1 - m2)
            y_int = m1 * (x_int - x1) + y1

        # Check if the intersection point falls within the line segments
        if (
            min(x1, x2) <= x_int <= max(x1, x2) and
            min(y1, y2) <= y_int <= max(y1, y2) and
            min(x3, x4) <= x_int <= max(x3, x4) and
            min(y3, y4) <= y_int <= max(y3, y4)
        ):
            return (round(x_int, 2), round(y_int, 2))
        else:
            return None  # Intersection point is outside the line segments

    def set_intersection(self,line_segment_1,line_segment_2):
        intersect_pt = Street_Graph.find_intersection(line_segment_1, line_segment_2)
        
        if intersect_pt is not None:
            if intersect_pt not in self.intersections_points:
                self.intersections_points.append(intersect_pt)
                self.vertices.add(intersect_pt)
            self.vertices.add(line_segment_1[0])
            self.vertices.add(line_segment_1[1])
            self.vertices.add(line_segment_2[0])
            self.vertices.add(line_segment_2[1])

            
            if tuple(line_segment_1) not in self.intersection_segments:
                self.intersection_segments[tuple(line_segment_1)]=[intersect_pt]
            else:
                if intersect_pt not in self.intersection_segments[tuple(line_segment_1)]:
                    self.intersection_segments[tuple(line_segment_1)].append(intersect_pt)
            if tuple(line_segment_2) not in self.intersection_segments:
                self.intersection_segments[tuple(line_segment_2)]=[intersect_pt]
            else:
                if intersect_pt not in self.intersection_segments[tuple(line_segment_2)]:
                    self.intersection_segments[tuple(line_segment_2)].append(intersect_pt)

    def generate_graph(self):

        vertex_map = {}
        
        for i in range(len(self.lines)):
            for j in range(i+1,len(self.lines)):
                if self.lines[i][0]!=self.lines[j][0]:
                    self.set_intersection(self.lines[i][1],self.lines[j][1])

        def distance(point1, point2):
            x1, y1 = point1
            x2, y2 = point2
            return sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2)
    
        for key, value in self.intersection_segments.items():
            value.sort(key=lambda point: distance(key[0], point))
        
        for k,v in self.intersection_segments.items():
            if((k[0],v[0]) not in self.edges and (v[0],k[0]) not in self.edges and (v[0]!=k[0])):
                self.edges.append((k[0],v[0]))
            for i in range(0,len(v)-1):
                if((v[i],v[i+1]) not in self.edges and (v[i+1],v[i]) not in self.edges  and (v[i]!=v[i+1])):				
                    self.edges.append((v[i],v[i+1]))
            if((k[1],v[len(v)-1]) not in self.edges and (v[len(v)-1],k[1]) not in self.edges  and (v[len(v)-1]!=k[1])):
                self.edges.append((k[1],v[len(v)-1]))
        
        # print("V = {", file=sys.stdout)
        ctr=1
        for vertex in self.vertices:
            # print("{0} : ({1},{2})".format(ctr, round(float(vertex[0]), 2), round(float(vertex[1]), 2)), file=sys.stdout) 
            ctr+=1
            vertex_map[ctr]=vertex
        # print("}", file=sys.stdout)
        print("V",len(self.vertices))
        vertex_map_values = list(vertex_map.values())
        print("E {", end='')
        for i, edge in enumerate(self.edges):
            if i == len(self.edges)-1:
                print("<{0},{1}>".format(vertex_map_values.index(edge[0])+1 , vertex_map_values.index(edge[1])+1), file=sys.stdout, end='')
            else:
                print("<{0},{1}>,".format(vertex_map_values.index(edge[0])+1 , vertex_map_values.index(edge[1])+1), file=sys.stdout, end='')
        print("}", file=sys.stdout)