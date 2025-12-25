from PIL import Image, ImageDraw, ImageFont

# Create a new image with dark background
width, height = 256, 256
img = Image.new('RGB', (width, height), color='#0a0e27')
draw = ImageDraw.Draw(img)

# Draw a rounded rectangle background
draw.rectangle([10, 10, 246, 246], fill='#1a1f3a', outline='#00ff00', width=3)

# Draw the snake (green squares)
snake_positions = [(80, 100), (100, 100), (120, 100), (140, 100)]
for x, y in snake_positions:
    draw.rectangle([x, y, x+20, y+20], fill='#00ff00', outline='#00dd00', width=1)

# Draw the food (red square)
food_x, food_y = 160, 140
draw.rectangle([food_x, food_y, food_x+20, food_y+20], fill='#ff0000', outline='#dd0000', width=1)

# Draw title text
try:
    # Try to use a bold font if available
    font = ImageFont.truetype("arial.ttf", 48)
except:
    font = ImageFont.load_default()

draw.text((40, 200), "SNAKE", fill='#00ff00', font=font)

# Save as PNG first (for quality)
img.save('game_icon.png')
print("✅ game_icon.png created successfully!")

# Convert to ICO (Windows icon)
img.save('game_icon.ico', sizes=[(256, 256), (128, 128), (64, 64), (32, 32), (16, 16)])
print("✅ game_icon.ico created successfully!")