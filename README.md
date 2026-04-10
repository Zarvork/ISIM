# ISIM

Impact of the different parameters of the simulation:

- mass => Mass of the particles.

- damping => Controls how quickly the simulation loses energy. The higher the value, the less energy the cloth loses.

- number of iterations of the constraint loop => Impacts the overall stiffness of the cloth. The higher the value, the more the cloth is stiff.

- width/height => Controls the size of the cloth.

- spacing => The distance between each particle.

- num_frames => Number of images you want to generate.

- time_between_image => Time interval between two generated images.

- delta_time => Time step of one physical step.

- nb_steps => Number of physical steps calculated between two images. The higher the value, the faster the simulation progresses visually. It is computed with : time_between_image / delta_time.