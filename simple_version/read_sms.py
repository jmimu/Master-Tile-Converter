#! /usr/bin/python
# -*- coding: utf-8 -*-

#    MASTER TILE CONVERTER
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


#with "Phantasy Star" RLE
#http://www.smspower.org/Development/Compression

if (len(sys.argv)<4):
  print("Synthax: python read_sms_3bpp.py (3bpp|4bpp|compr) romfile.sms offset_hex")
  print("Example for AlexKidd : python read_sms.py 3bpp ../\[BIOS\]\ Alex\ Kidd\ in\ Miracle\ World\ \(USA\,\ Europe\).sms 0x10341")
  exit()

if (sys.argv[1]!="3bpp")and(sys.argv[1]!="4bpp")and(sys.argv[1]!="compr"):
  print("You have to say \"3bpp\", \"4bpp\" or \"compr\".")
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


class Compressed_tiles(object):
  def __init__(self):
    self.tiles=[]
    self.uncompressed_bytes=[]#the 4 bitplanes
    self.uncompressed_data=[]
    for y in range(4):
      self.uncompressed_bytes.append([])

  #test if data at index is compressed
  def test_compressed(self,datarom,index): #return true if valid
    #checks if all bitplans are the same size => valid compressed data
    offset=0
    bytes_in_bitplan=[]
    print("Testing data at ",index,"...")
    for num_bitplane in range(4):
      bytes_in_bitplan.append(0)
      if (num_bitplane==1):
         if (bytes_in_bitplan[0]==0): #already false
             break
      if (num_bitplane==2):
         if (bytes_in_bitplan[0]!=bytes_in_bitplan[1]): #already false
             break
      if (num_bitplane==3):
         if (bytes_in_bitplan[1]!=bytes_in_bitplan[2]): #already false
             break
      #print("bitplane ",num_bitplane)
      while ((index+offset<len(romdata))and(datarom[index+offset]!=0)):
        if (datarom[index+offset]<128):#identical bytes
          nb_consecutive_identical_bytes=datarom[index+offset]
          #print("   found ",nb_consecutive_identical_bytes," identical bytes")
          bytes_in_bitplan[num_bitplane]+=nb_consecutive_identical_bytes
          offset+=1+1
          continue
        if (datarom[index+offset]>=128):#consecutive different tiles
          nb_consecutive_different_bytes=datarom[index+offset]-128
          #print("   found ",nb_consecutive_different_bytes," different bytes")
          bytes_in_bitplan[num_bitplane]+=nb_consecutive_different_bytes
          offset+=1+nb_consecutive_different_bytes
          continue
      offset+=1
    #print(bytes_in_bitplan)
    if (bytes_in_bitplan[0]>0)and(bytes_in_bitplan[0]==bytes_in_bitplan[1]==bytes_in_bitplan[2]==bytes_in_bitplan[3]):
      print("Found ",bytes_in_bitplan[0]*4," bytes compressed into ",offset," bytes.")
      return True#(offset-1,bytes_in_bitplan[0]*4)
    else:
      print("This is not compressed data.")
      return False

  def read_compressed(self,datarom,index): #return number of bytes read
    offset=0#where we are on the file after index
    
    #reset
    self.uncompressed_bytes=[]#the 4 bitplanes
    self.uncompressed_data=[]
    for y in range(4):
      self.uncompressed_bytes.append([])
    self.tiles=[]

    #uncompress data
    print("Uncompressing...")
    for num_bitplane in range(4):
     #print("bitplane ",num_bitplane)
      while (datarom[index+offset]!=0):
        if (datarom[index+offset]<128):#identical bytes
          nb_consecutive_identical_bytes=datarom[index+offset]
          offset+=1
          the_byte=datarom[index+offset]
          #print("   found ",nb_consecutive_identical_bytes," times ",the_byte)
          for i in range(nb_consecutive_identical_bytes):
            self.uncompressed_bytes[num_bitplane].append(the_byte)
          offset+=1
          continue
        if (datarom[index+offset]>=128):#consecutive different tiles
          nb_consecutive_different_bytes=datarom[index+offset]-128
          #print("   found ",nb_consecutive_different_bytes," different bytes:")
          for i in range(nb_consecutive_different_bytes):
            offset+=1
            the_byte=datarom[index+offset]
            #print("     ",the_byte)
            self.uncompressed_bytes[num_bitplane].append(the_byte)
          offset+=1
          continue
      offset+=1
    
    #print("uncompressed:")
    #print(self.uncompressed_bytes)
    
    
    #order bytes
    for i in range(len(self.uncompressed_bytes[0])):
      for b in range(4):#bitplane
        self.uncompressed_data.append(self.uncompressed_bytes[b][i])
    
    #print("in order:")
    #print(self.uncompressed_data)
    
    for i in range(len(self.uncompressed_bytes[0])/8):
      tile=Tile4bpp()
      self.tiles.append(tile)
    index2=0
    for tile in self.tiles:
      index2=tile.read(self.uncompressed_data,index2)
      tile.update_image(palette_alexsprite)
      
    print("Read ",offset-1," bytes.")
    return offset



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


if (sys.argv[1]=="compr"):
  compr=Compressed_tiles()
  while (index_init<len(romdata))and(not compr.test_compressed(romdata,index_init)):
    index_init+=1
  if (index_init<len(romdata)):
    compr.read_compressed(romdata,index_init)
else:
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
        if (sys.argv[1]!="compr"):
          for tile in tiles:
            index=tile.read(romdata,index)
            tile.update_image(palette_alexsprite)
      if event.key == K_DOWN:
        index_init+=384 #384 for 3bpp, 0x200 for 4bpp
        index=index_init
        print(index_init)
        if (sys.argv[1]!="compr"):
          for tile in tiles:
            index=tile.read(romdata,index)
            tile.update_image(palette_alexsprite)
      if ((sys.argv[1]=="compr")and(event.key == K_RIGHT)):#search for next compressed data
        index_init+=1
        while (index_init<len(romdata))and(not compr.test_compressed(romdata,index_init)):
          index_init+=1
        if (index_init<len(romdata)):
          compr.read_compressed(romdata,index_init)


  screen.fill(black)
  tiles_surface.fill(black)
  x=0
  y=0
  if (sys.argv[1]=="compr"):
    for tile in compr.tiles:
      tile.draw(tiles_surface,x*8,y*8)
      x+=1
      if (x>=16):
        x=0
        y+=1
  else:
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


