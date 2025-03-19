import struct
import os

raw_files = []
raw_file_paths = [
    # 'cannothide.raw', 
    'ashnazg.raw',
    'go_other_way.raw', 
    'keep_walking.raw',
    'ring_command.raw',
    'lering.raw', 
    'lering_activated.raw'
]
output_file_path = 'pcm_data.c'

# Read the raw PCM data
for raw_file_path in raw_file_paths:
    with open(raw_file_path, 'rb') as f:
        raw_data = f.read()
        raw_files.append((raw_file_path, raw_data))

# Write the PCM data as a C array to the output file
with open(output_file_path, 'w') as f:
    f.write('#include "pcm_data.h"\n\n')
    for raw_file_path, raw_data in raw_files:
        i = 0
        pcm_data = struct.unpack('<' + 'h' * (len(raw_data) // 2), raw_data)
        array_name = os.path.splitext(os.path.basename(raw_file_path))[0]
        f.write(f'const int16_t pcm_data_{array_name}[] = {{\n')
        for sample in pcm_data:
            if i == 32767:
                break
            f.write(f'    {sample},\n')
            i += 1
        f.write('};\n\n')

with open('pcm_data.h', 'w') as f:
    f.write('#include <stdint.h>\n\n')
    for raw_file_path, _ in raw_files:
        array_name = os.path.splitext(os.path.basename(raw_file_path))[0]
        f.write(f'extern const int16_t pcm_data_{array_name}[];\n')

print(f'PCM data written to {output_file_path}')