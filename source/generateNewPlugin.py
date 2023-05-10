import os
import jinja2

# define paths
TEMPLATE_DIR = './templates'
OUTPUT_SOURCE = './'
OUTPUT_ROOT = '../'

# create output directory if it does not exist
if not os.path.exists(OUTPUT_SOURCE):
    os.makedirs(OUTPUT_SOURCE)

# ask for user input
while True:
    plugin_name = input("Enter the plugin name using only lower and upper case characters: ")
    if all(c.islower() or c.isupper() for c in plugin_name):
        break
    print("Error: plugin name can only contain lower and upper case characters. Please try again.")
plugin_uri = input("Enter the plugin URI (e.g. https://example.com): ")
class_name = plugin_name[:1].upper() + plugin_name[1:]
file_name = plugin_name[:1].lower() + plugin_name[1:]
define_name = plugin_name.upper()
project_name = 'sve-' + plugin_name

# load templates
env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATE_DIR))

# render plugin.cpp
template = env.get_template('plugin.cpp.template')
plugin_cpp = template.render(class_name=class_name, file_name=file_name)

# write plugin.cpp to file
filename = f'{file_name}.cpp'
with open(os.path.join(OUTPUT_SOURCE, filename), 'w') as f:
    f.write(plugin_cpp)

# render plugin.hpp
template = env.get_template('plugin.hpp.template')
plugin_hpp = template.render(class_name=class_name,
                             define_name=define_name,
                             plugin_name=plugin_name,
                             project_name=project_name)

# write plugin.hpp to file
filename = f'{file_name}.hpp'
with open(os.path.join(OUTPUT_SOURCE, filename), 'w') as f:
    f.write(plugin_hpp)

# render DistrhoPluginInfo.h
template = env.get_template('DistrhoPluginInfo.h.template')
DistrhoPluginInfo_h = template.render(project_name=project_name, plugin_uri=plugin_uri)

# write DistrhoPluginInfo.h
with open(os.path.join(OUTPUT_SOURCE, 'DistrhoPluginInfo.h'), 'w') as f:
    f.write(DistrhoPluginInfo_h)

# render CMakeLists.txt
template = env.get_template('CMakeLists.txt.template')
CMakeLists_txt = template.render(project_name=project_name, file_name=file_name)

# write CMakeLists.txt
with open(os.path.join(OUTPUT_ROOT, 'CMakeLists.txt'), 'w') as f:
    f.write(CMakeLists_txt)

# render README.md
template = env.get_template('README.md.template')
README_md = template.render(project_name=project_name, file_name=file_name)

# write README.md
with open(os.path.join(OUTPUT_ROOT, 'README.md'), 'w') as f:
    f.write(README_md)

print("Plugin generated successfully!")
