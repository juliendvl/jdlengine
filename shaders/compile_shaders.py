import logging
import os
import subprocess


# Logger configuration
logging.basicConfig(level=logging.INFO, format="[%(levelname)s] %(message)s")


def find_slang_compiler() -> str:
    """Finds and returns the path of the Slang compiler."""
    vulkan_path: str = os.environ.get("VULKAN_SDK")
    if not os.path.exists(vulkan_path):
        vulkan_path = os.environ.get("VK_SDK_PATH")
    if not os.path.exists(vulkan_path):
        raise RuntimeError("Cannot find the Vulkan SDK directory!")
    
    slang_compiler: str = os.path.join(vulkan_path, "Bin", "slangc.exe")
    if not os.path.exists(slang_compiler):
        raise RuntimeError("Cannot find the Slang compiler!")
    
    return slang_compiler


def compile_shaders() -> None:
    """Compiles all the Slang shaders located in the script directory."""
    slang_compiler: str = find_slang_compiler()

    shaders_root_path: str = os.path.dirname(__file__)
    for file in os.listdir(shaders_root_path):
        file_name, extension = os.path.splitext(file)
        if extension != ".slang":
            continue

        slang_path: str = os.path.join(shaders_root_path, file)
        spv_path: str = os.path.join(shaders_root_path, f"{file_name}.spv")

        logging.info(f"Compiling {slang_path}...")
        subprocess.Popen([
            slang_compiler,
            slang_path,
            "-target", "spirv",
            "-profile", "spirv_1_4",
            "-emit-spirv-directly",
            "-fvk-use-entrypoint-name",
            "-entry", "vertMain",
            "-entry", "fragMain",
            "-o", spv_path
        ])
    
    logging.info("DONE!")


if __name__ == "__main__":
    compile_shaders()
