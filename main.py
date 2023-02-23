# importing image object from PIL
import pygame
from PIL import Image, ImageDraw

WIDTH = 400
HEIGHT = 400

# def collide_rect(a_x, a_y, a_width, a_height, b_x, b_y, b_width, b_height):
#     return (a_x + a_width > b_x) and (a_x < b_x + b_width) and (a_y + a_height > b_y) and (a_y < b_y + b_height)

# pygame.init()

# screen = pygame.display.set_mode((WIDTH, HEIGHT))

# balls=[]
# tiles=[]

# tiles_spawned = False

# screen = pygame.display.set_mode((WIDTH, HEIGHT))
# pygame.display.set_caption('Pixel Art! :)')
# screen.fill((0,0,0))
# pygame.display.flip()
# running = True

# spp = None
# select_pos = [pygame.mouse.get_pos()[0], pygame.mouse.get_pos()[1]]

# clock = pygame.time.Clock()
# font = pygame.font.SysFont("Arial", 18)
 
 
# def update_fps():
# 	fps = str(int(clock.get_fps()))
# 	fps_text = font.render(fps, 1, pygame.Color("coral"))
# 	return fps_text
# while running: 
#     clock.tick(60)
#     screen.fill((0,0,0))

#     select_pos = [pygame.mouse.get_pos()[0]-50/2, pygame.mouse.get_pos()[1]-50/2]

#     screen.blit(update_fps(), (10,0))

#     #pygame.draw.

#     for oy in range(8):
#         y = oy
#         for ox in range(8):
#             x = ox

#             new_x = x * 50
#             new_y = y * 50

#             # pygame.draw.rect(screen, (48, 48, 48),
#             #     pygame.Rect(new_x, new_y, 50, 50)
#             # )
#             if not tiles_spawned:
#                 tiles_spawned = True
#                 tiles.append((new_x, new_y))
#             if collide_rect(new_x, new_y, 50, 50, pygame.mouse.get_pos()[0], pygame.mouse.get_pos()[1], 16, 16):
#                 select_pos = [new_x, new_y]

#     for ball in balls:
#         pygame.draw.rect(screen, (255, 0, 0),
#             rect=pygame.Rect(ball[0], ball[1], 50, 50)
#         )
#         for tile in tiles:
#             if collide_rect(tile[0], tile[1], 50, 50, ball[0], ball[1], 50, 50):
#                 #print("hi")
#                 ball[0] = tile[0]
#                 ball[1] = tile[1]

#     pygame.draw.rect(screen, (255, 0, 0), pygame.Rect(select_pos[0], select_pos[1], 50, 50))

#     for event in pygame.event.get():
#         if event.type == pygame.QUIT:
#             running = False
#         if event.type == pygame.MOUSEBUTTONDOWN:
#             ball = pygame.mouse.get_pos()
#             if event.button == 1:
#                 if ball > (350, 350):
#                     ball = (350, 350)
#                 elif ball < (0, 0):
#                     ball = (0, 0)
#                 balls.append([ball[0], ball[1]])

#     pygame.display.update()

    
  
w, h = 400, 400
shape = [(0, 0), (1, 1)]
  
# creating new Image object
img = Image.new("RGB", (w, h))
# create rectangle image
img1 = ImageDraw.Draw(img)

file = open("art.txt", "r")
fl = file.readlines()
for line in fl:
    print(line)
    lball = line.split()
    print(lball)
    dball = (int(lball[0]), int(lball[1]), int(lball[0])+50, int(lball[1])+50)
    img1.rectangle(dball, (int(lball[2]), int(lball[3]), int(lball[4])))

file.close()

img.save('art.png')
img.show('art.png')