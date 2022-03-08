import os
import shutil

os.chdir(os.path.dirname(os.path.realpath(__file__)))
os.makedirs("Final", exist_ok = True)

shutil.copy("Aether.exe", "Final/Aether.exe")
shutil.copy("README.md", "Final/README.md")
shutil.copy("LICENSE.txt", "Final/LICENSE.txt")
shutil.copytree("Shaders", "Final/Shaders", dirs_exist_ok = True)
shutil.copytree("licenses", "Final/licenses", dirs_exist_ok = True)

shutil.make_archive("Release", 'zip', "Final")