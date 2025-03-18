import struct
import os

raw_files = []
raw_file_paths = ['cannothide.raw', 'ashnazg.raw']
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
        pcm_data = struct.unpack('<' + 'h' * (len(raw_data) // 2), raw_data)
        array_name = os.path.splitext(os.path.basename(raw_file_path))[0]
        f.write(f'const int16_t pcm_data_{array_name}[] = {{\n')
        for sample in pcm_data:
            f.write(f'    {sample},\n')
        f.write('};\n\n')

print(f'PCM data written to {output_file_path}')