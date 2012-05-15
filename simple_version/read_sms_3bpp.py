#! /usr/bin/python
# -*- coding: utf-8 -*-

#    ???
#    Copyright (C) 2012  JM Muller
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.

import pygame
import os

import pygame, sys, time, random
from pygame.locals import *

#add a readme
#parameters : rom filename, offset
#add 3bpp/4bpp key
#faster: http://www.pygame.org/docs/tut/surfarray/SurfarrayIntro.html



if (len(sys.argv)<4):
  print("Synthax: python (3bpp|4bpp) main.py romfile.sms offset_hex")
  print("Example for AlexKidd : python main.py 3bpp ../\[BIOS\]\ Alex\ Kidd\ in\ Miracle\ World\ \(USA\,\ Europe\)_JM.sms 0x10341")
  exit()

if (sys.argv[1]!="3bpp")and(sys.argv[1]!="4bpp"):
  print("You have to say \"3bpp\" or \"4bpp\".")
  exit()

filename=sys.argv[2]
index_init=int(sys.argv[3],16)



palette_alexsprite=[] #alex kidd 3bpp palette (+8 gray for 4bpp)
palette_alexsprite.append( (0xFF,0x00,0x00) )
palette_alexsprite.append( (0xFF,0xFF,0xFF) )
palette_alexsprite.append( (0x00,0x55,0x55) )
palette_alexsprite.append( (0x00,0xAA,0xFF) )
palette_alexsprite.append( (0x00,0x00,0xFF) )
palette_alexsprite.append( (0x00,0x00,0xAA) )
palette_alexsprite.append( (0x00,0x00,0x00) )
palette_alexsprite.append( (0xFF,0x00,0x00) )
palette_alexsprite.append( (0x10,0x10,0x10) )#only for 4bpp
palette_alexsprite.append( (0x30,0x30,0x30) )
palette_alexsprite.append( (0x50,0x50,0x50) )
palette_alexsprite.append( (0x70,0x70,0x70) )
palette_alexsprite.append( (0x90,0x90,0x90) )
palette_alexsprite.append( (0xb0,0xb0,0xb0) )
palette_alexsprite.append( (0xd0,0xd0,0xd0) )
palette_alexsprite.append( (0xF0,0xF0,0xF0) )

palette_tlp=[] #tile layer pro default palette
palette_tlp.append( (0,0,0) )
palette_tlp.append( (100,200,100) )
palette_tlp.append( (55,50,255) )
palette_tlp.append( (220,255,255) )
palette_tlp.append( (50,0,134) )
palette_tlp.append( (191,155,0) )
palette_tlp.append( (0,207,255) )
palette_tlp.append( (239,235,180) )
palette_tlp.append( (147,0,0) )
palette_tlp.append( (81,255,0) )
palette_tlp.append( (255,172,0) )
palette_tlp.append( (188,17,164) )
palette_tlp.append( (0,0,0) )
palette_tlp.append( (89,140,242) )
palette_tlp.append( (182,0,159) )
palette_tlp.append( (131,220,0) )





#a tile is a 8x8 pixels image. Stored in indexed colors
class Tile(object):
  def __init__(self):
    self.image=pygame.image.load("empty.png")
    #self.image = pygame.Surface( (8,8) )
    self.data=[]
    for y in range(8):
      self.data.append([])
      for x in range(8):
        self.data[y].append(0)
  
  def read(self,romdata,index): #return final index (for next tile)
    for i in range(8):
      index+=self.read_8pixels(romdata,index,self.data[i])
    return index
  
  def read_8pixels(self,rom,index,line_data): #return number of bytes read
    raise NotImplementedError
  
  def update_image(self,palette):
    for y in range(8):
      for x in range(8):
        self.image.set_at((x,y),palette[self.data[y][x]])
  
  def draw(self,surface,x,y):
    surface.blit(self.image, (x, y))

  


class Tile3bpp(Tile):
  def __init__(self):
    Tile.__init__(self)
  def read_8pixels(self,datarom,index,line_data): #return number of bytes read
    for j in range(8):
      line_data[j]=0
    shift=1
    for i in range(3):
        line_data[0]+=( (datarom[index+i]/128)%2 ) * shift
        line_data[1]+=( (datarom[index+i]/64)%2 ) * shift
        line_data[2]+=( (datarom[index+i]/32)%2 ) * shift
        line_data[3]+=( (datarom[index+i]/16)%2 ) * shift
        line_data[4]+=( (datarom[index+i]/8)%2 ) * shift
        line_data[5]+=( (datarom[index+i]/4)%2 ) * shift
        line_data[6]+=( (datarom[index+i]/2)%2 ) * shift
        line_data[7]+=( (datarom[index+i]/1)%2 ) * shift
        shift*=2
    return 3

class Tile4bpp(Tile):
  def __init__(self):
    Tile.__init__(self)
  def read_8pixels(self,datarom,index,line_data): #return number of bytes read
    for j in range(8):
      line_data[j]=0
    shift=1
    for i in range(4):
        line_data[0]+=( (datarom[index+i]/128)%2 ) * shift
        line_data[1]+=( (datarom[index+i]/64)%2 ) * shift
        line_data[2]+=( (datarom[index+i]/32)%2 ) * shift
        line_data[3]+=( (datarom[index+i]/16)%2 ) * shift
        line_data[4]+=( (datarom[index+i]/8)%2 ) * shift
        line_data[5]+=( (datarom[index+i]/4)%2 ) * shift
        line_data[6]+=( (datarom[index+i]/2)%2 ) * shift
        line_data[7]+=( (datarom[index+i]/1)%2 ) * shift
        shift*=2
    return 4




pygame.init()

size = width, height = 800, 600
speed = [2, 2]
black = 0, 0, 0

screen = pygame.display.set_mode(size)
tiles_surface = pygame.Surface( (16*8,32*8) )
tiles_surface_zoomed = pygame.Surface( (16*8*2,32*8*2) )

romdata=[]
print("Reading file...")
f = open(filename, "rb")
try:
  byte = f.read(1)
  while byte != "":
    romdata.append(ord(byte))
    byte = f.read(1)
finally:
    f.close()
print("Done. Got ",len(romdata)," bytes")


print("Init...")

tiles=[]
for i in range(16*16):
  if (sys.argv[1]=="3bpp"):
    tile=Tile3bpp()
  else:
    tile=Tile4bpp()
  tiles.append(tile)

print ("Read tiles")



index=index_init
for tile in tiles:
  index=tile.read(romdata,index)
  tile.update_image(palette_alexsprite)


#print(tiles[0].data)

print("Done.")

while 1:
  for event in pygame.event.get():
    if event.type == QUIT:
      sys.exit()
    elif event.type == KEYDOWN:
      if event.key == K_UP:
        index_init-=384 #384 for 3bpp, 0x200 for 4bpp
        if (index_init<0):
          index_init=0
        index=index_init
        print(index_init)
        for tile in tiles:
          index=tile.read(romdata,index)
          tile.update_image(palette_alexsprite)
      if event.key == K_DOWN:
        index_init+=384 #384 for 3bpp, 0x200 for 4bpp
        index=index_init
        print(index_init)
        for tile in tiles:
          index=tile.read(romdata,index)
          tile.update_image(palette_alexsprite)

  screen.fill(black)
  x=0
  y=0
  for tile in tiles:
    tile.draw(tiles_surface,x*8,y*8)
    x+=1
    if (x>=16):
      x=0
      y+=1
  tiles_surface_zoomed = pygame.transform.scale(tiles_surface,(16*8*4,32*8*4))
  screen.blit(tiles_surface_zoomed, (0, 0))
  pygame.display.flip()
  pygame.time.delay(100)


