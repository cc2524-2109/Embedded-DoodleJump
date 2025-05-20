import pygame 
import serial
import random

y_change = 0
score = 0
buttons = serial.Serial(port='COM3', baudrate=115200)

class Doodle:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.jump = False

def initialize_game(screen_width = 400, screen_height = 700):
    pygame.init()
    screen = pygame.display.set_mode([screen_width, screen_height])
    pygame.display.set_caption('Doodle Jump')
    avatar = pygame.transform.scale(pygame.image.load('avatar.png'), (50, 50))
    avatar_location = Doodle(175, 625)
    return (screen, avatar, avatar_location)


def update_y_location(avatar):
    global y_change
    jump_height = 12
    gravity = 0.6
    if avatar.jump:
        y_change = -jump_height
        avatar.jump = False
    avatar.y += y_change
    y_change += gravity
    return(avatar.y, avatar.jump)

def avatar_block_collision(plat_list, avatar):
    global y_change
    for i in range(len(plat_list)):
        if (plat_list[i].colliderect([avatar.x, avatar.y + 55, 45, 10]) 
            and avatar.jump == False and y_change > 0):
            avatar.jump = True
    return(avatar.jump)
        
def handle_serial_input(avatar_location, serial):
    x_location = avatar_location.x 
    if serial.in_waiting > 0:
        serial_data = serial.readline(1).decode().strip()
        x_location = handle_serial_data(serial_data, x_location)
    return(x_location)


def handle_serial_data(serial_data, avatar_x):
    if serial_data == 'l':
        avatar_x -= 20
    elif serial_data == 'r':
        avatar_x += 20
    return(avatar_x)
        
def update_platforms(plat_list, change, y_location, buttons, level): 
    global score  
    def lowest_y_and_x():
        y_loc = float("inf")
        x_loc = -1
        for p in plat_list:
            if p[1] < y_loc:
                y_loc = p[1]
                x_loc = p[0]
        return x_loc, y_loc
    boundary = 450 + (5 * level)

    if y_location < boundary and change < 0:
        for i in range(len(plat_list)):
            plat_list[i][1] -= y_change
    else:
        pass
    for j in range(len(plat_list)):
        if plat_list[j][1] > 700:
            score += 1
            send_score_to_lcd(score, buttons)  
            baseline_x, baseline_y = lowest_y_and_x()
            if random.randint(1, 4) == 1:
                plat_list[j] = [330 - baseline_x, baseline_y, 70, 10]
            else:
                if baseline_x == 0:
                    plat_list[j] = [baseline_x + random.randint(100, 150), baseline_y - random.randint(110, 130), 70, 10]
                elif baseline_x == 330:
                    plat_list[j] = [baseline_x + random.randint(-150, -100), baseline_y - random.randint(110, 130), 70, 10]
                else:
                    platform_change = -1
                    if random.randint(1,2) == 1:    
                        platform_change = random.randint(-150, -50)
                    else:
                        platform_change = random.randint(50, 150)
                    plat_list[j] = [max(0, min(330, baseline_x + platform_change)), baseline_y - random.randint(110, 130), 70, 10]
    return(plat_list)

def send_score_to_lcd(score, buttons):
    score_string = f"{score}\n"
    buttons.write(score_string.encode())

def main():
    global buttons
    global y_change
    global score
    global level 

    #Running Main
    running = True
    
    background_color = (255, 255, 255)
    platform_color = (124, 252, 0)

    frame_rate = 60

    platforms = [[165, 680, 70, 10], [85, 570, 70, 10], [265, 570, 70, 10], [165, 460, 70, 10], 
                 [85, 350, 70, 10], [265, 350, 70, 10], [165, 240, 70, 10], [85, 140, 70, 10], 
                 [265, 140, 70, 10], [165, 70, 70, 10], [85, 0, 70, 10], [265, 0, 70, 10], [165, -70, 70, 10]]

    clock = pygame.time.Clock()

    screen, avatar, avatar_location = initialize_game()

    while running == True:
        level = int(score / 10)

        screen.fill(background_color)
        screen.blit(avatar, (avatar_location.x, avatar_location.y))
        
        font = pygame.font.Font(None, 20)
        score_text = font.render("Score: " + str(score), True, (255, 0, 0))
        screen.blit(score_text, (10, 10))

        font = pygame.font.Font(None, 20)
        score_text = font.render("Level: " + str(level), True, (255, 0, 0))
        screen.blit(score_text, (10, 25))

        landings = []

        #Platforms 
        for i in range(len(platforms)):
            land = pygame.draw.rect(screen, platform_color, platforms[i], 0, 3)
            landings.append(land)

        #Update Y
        avatar_location.y, avatar_location.jump = update_y_location(avatar_location)

        #Check Collision 
        avatar_location.jump = avatar_block_collision(landings, avatar_location)
            
        #Update X
        avatar_location.x = handle_serial_input(avatar_location, buttons)

        #Update Platorms
        platforms = update_platforms(platforms, y_change, avatar_location.y, buttons, level)

        #Exit 
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False  
        
        if avatar_location.y > 650:
            y_change = 0
            running = False
        
        if avatar_location.x < 20:
            avatar_location.x = 20
        elif avatar_location.x > 330:
            avatar_location.x = 330

        clock.tick(frame_rate)
        pygame.display.flip()

    gameover = font.render("Press Right to Respawn and Left to Leave", False, (255,0,0))
    rect = gameover.get_rect()
    rect.center = screen.get_rect().center
    screen.blit(gameover, rect)
    pygame.display.flip()

    done = False 
    while not done:
        count = 0 
        while count < 350:
            count = count + 1
        if buttons.in_waiting > 0:
            serial_data = buttons.readline(1).decode().strip()
            if serial_data == 'l':
                done = True
            elif serial_data == 'r':
                score = 0
                main()
        
    pygame.quit()

if __name__ == "__main__":
    main()
