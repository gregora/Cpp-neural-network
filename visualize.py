from Tkinter import *
import sys
import json
import time

canvas_width = 1000
canvas_height = 600


file_path = raw_input("What is the file path? ")


tk = Tk()
tk.title(file_path)
w = Canvas(tk, width=canvas_width, height=canvas_height)
w.pack()


def main():

    file = open(file_path, "r")
    l1 = file.read()
    l1 = l1.replace("\n", " ")
    file_vector = l1.split(" ")

    input_edges = []
    hidden_edges = []
    output_edges = []


    x = -1
    y = -1

    #set input edges
    while True:
        word = file_vector[0]
        if(word != "newline"):
          if(word == "["):
            y = y + 1
            x = 0
            input_edges.append([])
          elif (word == ""):
            pass
          elif(word != "]"):
            x = x + 1
            input_edges[y].append(float(word))

          del file_vector[0]
        else:
          del file_vector[0]
          break





    x = -1
    y = -1
    z = 0
    hidden_edges.append([])
    #set hidden nodes
    while (True):
        word = file_vector[0]

        if(word != "newline"):

          if(word == "newlayer"):
            y = -1
            x = 0
            z = z + 1
            hidden_edges.append([])
          else:
            if(word == "["):
              y = y + 1
              x = 0
              hidden_edges[z].append([])
            elif(word == ""):
              pass

            elif(word != "]"):
              x = x + 1
              hidden_edges[z][y].append(float(word))

          del file_vector[0]
        else:
          del file_vector[0]
          break

    if(hidden_edges == [[]]):
        hidden_edges = []

    x = -1
    y = -1
    #set output edges
    while(True):
        word = file_vector[0]
        if(word != "end"):
          if(word == "["):
            y = y + 1
            x = 0
            output_edges.append([])
          elif (word == ""):
            pass
          elif(word != "]"):
            x = x + 1
            output_edges[y].append(float(word))
          del file_vector[0]
        else:
          del file_vector[0]
          break

    circle_diameter = 40
    circle_radius = circle_diameter/2

    sections_num = (1 + len(hidden_edges) + 1) + 2
    section_width = canvas_width / sections_num

    inpnodes = len(input_edges)
    hidnodes = len(input_edges[0])
    outnodes = len(output_edges[0])

    maxnodes = max(inpnodes, hidnodes, outnodes)

    section_height = canvas_height / maxnodes




    #render all nodes and edges
    for i in range(0, inpnodes):
        w.create_oval(section_width, 40 + i*section_height, section_width + circle_diameter, circle_diameter + i*section_height + circle_diameter, fill="black")

        for j in range(0, len(input_edges[i])):
            w.create_line(circle_radius + section_width, circle_radius + 40 + i*section_height, circle_radius + 2*section_width, circle_radius + 40 + j*section_height, width=input_edges[i][j])


    for i in range(0, len(hidden_edges) + 1):

        for j in range(0, hidnodes):
            w.create_oval((i + 2)*section_width, 40 + j*section_height, (i + 2)*section_width + circle_diameter, circle_diameter + j*section_height + circle_diameter, fill="black")

            for m in range(0, hidnodes):
                if(len(hidden_edges) > i):
                    w.create_line(section_width*(2 + i) + circle_radius, 40 + j*section_height + circle_radius, section_width*(2 + 1 + i) + circle_radius, 40 + section_height*m + circle_radius, width=hidden_edges[i][j][m])

    for i in range(0, outnodes):

        w.create_oval((len(hidden_edges) + 3)*section_width, 40 + i*section_height, (len(hidden_edges) + 3)*section_width + circle_diameter, circle_diameter + i*section_height + circle_diameter, fill="black")

        for j in range(0, hidnodes):
            w.create_line(circle_radius + (len(hidden_edges) + 2)*section_width, 40 + (section_height)*j + circle_radius, circle_radius + (len(hidden_edges) + 3)*section_width, 40 + section_height*i + circle_radius, width=output_edges[j][i])
            pass

    w.mainloop()

main()
