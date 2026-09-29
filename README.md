# ISIM

## Impact of the different parameters of the simulation:

- mass => Mass of the particles.

- damping => Controls how quickly the simulation loses energy. The higher the value, the less energy the cloth loses.

- number of iterations of the constraint loop => Impacts the overall stiffness of the cloth. The higher the value, the more the cloth is stiff.

- width/height => Controls the size of the cloth.

- spacing => The distance between each particle.

- num_frames => Number of images you want to generate.

- time_between_image => Time interval between two generated images.

- delta_time => Time step of one physical step.

- nb_steps => Number of physical steps calculated between two images. The higher the value, the faster the simulation progresses visually. It is computed with : time_between_image / delta_time.

- is_xz_plane => When true, the cloth is in XZ plane. When false, the cloth is in XY plane.

## Demo

<img width="400" height="400" alt="cloth_4_corners_pinned" src="https://github.com/user-attachments/assets/c4a1e81e-9219-470a-9bfc-a9b5942d7a3c" />

<img width="400" height="400" alt="cloth_wind" src="https://github.com/user-attachments/assets/276fe13d-f866-452b-a586-3918ba0b2a74" />

<img width="400" height="400" alt="sphere_collision" src="https://github.com/user-attachments/assets/cf5e0218-ffed-43bf-9210-1e2bdbb2b178" />
