from matplotlib import pyplot as plt
# 创建STM32G474芯片图示
fig, ax = plt.subplots(figsize=(8, 8))

# 绘制芯片主体
chip_body = patches.Rectangle((2, 2), 4, 4, linewidth=2, edgecolor='black', facecolor='lightgray')
ax.add_patch(chip_body)

# 绘制芯片上的引脚 (总共48个引脚，每边12个)
pin_length = 0.5
num_pins_per_side = 12
pin_spacing = 4 / (num_pins_per_side - 1)
pin_positions_left = [(2 - pin_length, 2 + i * pin_spacing) for i in range(num_pins_per_side)]
pin_positions_right = [(6, 2 + i * pin_spacing) for i in range(num_pins_per_side)]
pin_positions_top = [(2 + i * pin_spacing, 6) for i in range(num_pins_per_side)]
pin_positions_bottom = [(2 + i * pin_spacing, 2 - pin_length) for i in range(num_pins_per_side)]

# 绘制左侧引脚
for pos in pin_positions_left:
    ax.add_patch(patches.Rectangle((pos[0], pos[1] - 0.1), pin_length, 0.2, linewidth=1, edgecolor='black', facecolor='darkgray'))
    
# 绘制右侧引脚
for pos in pin_positions_right:
    ax.add_patch(patches.Rectangle((pos[0] - pin_length, pos[1] - 0.1), pin_length, 0.2, linewidth=1, edgecolor='black', facecolor='darkgray'))
    
# 绘制顶部引脚
for pos in pin_positions_top:
    ax.add_patch(patches.Rectangle((pos[0] - 0.1, pos[1]), 0.2, pin_length, linewidth=1, edgecolor='black', facecolor='darkgray'))
    
# 绘制底部引脚
for pos in pin_positions_bottom:
    ax.add_patch(patches.Rectangle((pos[0] - 0.1, pos[1]), 0.2, pin_length, linewidth=1, edgecolor='black', facecolor='darkgray'))

# 绘制芯片上的标记
ax.text(4, 4, 'STM32G474', fontsize=12, ha='center', va='center', color='black')

# 设置绘图范围和隐藏坐标轴
ax.set_xlim(0, 8)
ax.set_ylim(0, 8)
ax.axis('off')

# 显示绘图
plt.show()

