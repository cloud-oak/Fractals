import sdl2, sdl2/gfx
import complex

discard sdl2.init(INIT_EVERYTHING)

const
    MAX_ITERATIONS = 50

    MIN_X = -2
    MAX_X = 1

    MIN_Y = -1
    MAX_Y = 1

    WIDTH = 1200
    HEIGHT = 800

    NUM_THREADS = 8

discard sdl2.init(INIT_VIDEO)

var
    iters: array[0..WIDTH, array[0..HEIGHT, uint8]]

    event = sdl2.defaultEvent

    window = createWindow("Mandelbrot", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_SHOWN)
    renderer = createRenderer(window, -1, RENDERER_ACCELERATED)

    quit = false
    invalid = true

    fpsman: FpsManager

fpsman.init
renderer.setDrawColor(0, 0, 0, 255)
renderer.clear()
renderer.present()

proc calcDot(x, y: int): uint8 = 
    var c : Complex = (toFloat(x) / WIDTH  * (MAX_X - MIN_X) + MIN_X,
                       toFloat(y) / HEIGHT * (MAX_Y - MIN_Y) + MIN_Y)
    var z = c
    while result < MAX_ITERATIONS:
        if abs(z) >= 4:
            return
        z = z * z + c
        result += 1


while not quit:
    while pollEvent(event):
        if event.kind == QuitEvent:
            quit = true
            break
    if invalid:
        for x in 0..<WIDTH:
            for y in 0..<HEIGHT:
                iters[x][y] = calcDot(x, y)

        renderer.setDrawColor(0, 0, 0, 255)
        renderer.clear()

        for x in 0..<WIDTH:
            for y in 0..<HEIGHT:
                renderer.setDrawColor(uint8(255 * float(iters[x][y]) / MAX_ITERATIONS),
                                      0,
                                      uint8(255 - (255 / MAX_ITERATIONS) * float(iters[x][y])))
                renderer.drawPoint(cint(x), cint(y))
    
        renderer.present()
        invalid = false
    fpsman.delay

destroy renderer
destroy window