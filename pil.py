from PIL import Image, ImageDraw

graphs = [
    "graphs/heatmaps/Heatmap_L1_vs_RenderTime_cornell-box.png",
    "graphs/heatmaps/Heatmap_L1_vs_RenderTime_dragon.png",
    "graphs/heatmaps/Heatmap_L1_vs_RenderTime_split-left.png",
    "graphs/heatmaps/Heatmap_L1_vs_RenderTime_split-right.png",
    "graphs/heatmaps/Heatmap_L1_vs_RenderTime_split.png",
    "graphs/heatmaps/Heatmap_L1_vs_RenderTime_triple-al.png",
    "graphs/heatmaps/Heatmap_L1_vs_RenderTime_triple.png",
]

cornell = [
    "graphs/TEST-cornell-box-direct_samples.png",
    "graphs/TEST-cornell-box-indirect_samples.png",
    "graphs/TEST-cornell-box-recursion_depth.png",
    "graphs/TEST-cornell-box-samples_pp.png"
]

dragon = [
    "graphs/TEST-dragon-direct_samples.png",
    "graphs/TEST-dragon-indirect_samples.png",
    "graphs/TEST-dragon-recursion_depth.png",
    "graphs/TEST-dragon-samples_pp.png"
]

split_left = [
    "graphs/TEST-split-left-direct_samples.png",
    "graphs/TEST-split-left-indirect_samples.png",
    "graphs/TEST-split-left-recursion_depth.png",
    "graphs/TEST-split-left-samples_pp.png"
]

split_right = [
    "graphs/TEST-split-right-direct_samples.png",
    "graphs/TEST-split-right-indirect_samples.png",
    "graphs/TEST-split-right-recursion_depth.png",
    "graphs/TEST-split-right-samples_pp.png"
]

split = [
    "graphs/TEST-split-direct_samples.png",
    "graphs/TEST-split-indirect_samples.png",
    "graphs/TEST-split-recursion_depth.png",
    "graphs/TEST-split-samples_pp.png"
]

triple_al = [
    "graphs/TEST-triple-al-direct_samples.png",
    "graphs/TEST-triple-al-indirect_samples.png",
    "graphs/TEST-triple-al-recursion_depth.png",
    "graphs/TEST-triple-al-samples_pp.png"
]

triple = [
    "graphs/TEST-triple-direct_samples.png",
    "graphs/TEST-triple-indirect_samples.png",
    "graphs/TEST-triple-recursion_depth.png",
    "graphs/TEST-triple-samples_pp.png"
]

#images = [Image.open(filename) for filename in graphs]
#images = [Image.open(filename) for filename in cornell]
#images = [Image.open(filename) for filename in dragon]
#images = [Image.open(filename) for filename in split_left]
#images = [Image.open(filename) for filename in split_right]
#images = [Image.open(filename) for filename in split]
#images = [Image.open(filename) for filename in triple_al]
images = [Image.open(filename) for filename in triple]

grid_size = (2, 2)
cell_width, cell_height = images[0].size
spacing = 10
canvas_width = grid_size[0] * cell_width + (grid_size[0] - 1) * spacing
canvas_height = grid_size[1] * cell_height + (grid_size[1] - 1) * spacing
canvas = Image.new('RGB', (canvas_width, canvas_height), (255, 255, 255))

for row in range(grid_size[1]):
    for col in range(grid_size[0]):
        image_index = row * grid_size[0] + col
        if image_index < len(images):
            x_offset = col * (cell_width + spacing)
            y_offset = row * (cell_height + spacing)
            canvas.paste(images[image_index], (x_offset, y_offset))

canvas.save('triple.png')