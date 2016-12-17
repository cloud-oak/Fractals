require "complex"

WIDTH = 200
HEIGHT = 50

MINX = -2.4
MAXX =  1.0
MINY = -1.2
MAXY =  1.2

VALS = [" "] * 6 + %w{ ░ ░ ░ ▒ ▒ ▒ ▓ ▓ ▓ █ █ █}

def trace_pixel(z)
	escape_time = 0
	c = z
	while z.abs < 2 && escape_time < VALS.size - 1
		z = z*z + c
		escape_time += 1
	end
	return VALS[escape_time]
end

def main()
	# Init
	x_vals = (0...WIDTH).to_a
	y_vals = (0...HEIGHT).to_a

	z_vals = y_vals.product(x_vals).map do |y,x|
		Complex.new(
			MINX + (MAXX - MINX) * x / WIDTH,
			MINY + (MAXY - MINY) * y / HEIGHT
		)
	end
	quit = false
	invalid = true
	until quit
		sleep 10.milliseconds
		next unless invalid
		# Calc
		output = ""
		z_vals.each_with_index do |z, i|
			output += '\n' if i % WIDTH == 0
			output += trace_pixel(z)
		end
		puts output
		invalid = false
	end
end

main
