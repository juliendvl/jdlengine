import argparse
import os
import subprocess


def find_spirv_compiler() -> str:
    """Finds and returns the path of the glslc SPIR-V compiler."""
    vulkan_sdk_path: str = os.environ.get("VK_SDK_PATH")
    if not os.path.exists(vulkan_sdk_path):
        vulkan_sdk_path: str = os.environ.get("VULKAN_SDK")
    if not os.path.exists(vulkan_sdk_path):
        raise RuntimeError("Cannot find the Vulkan SDK path.")

    compiler_path: str = os.path.join(vulkan_sdk_path, "bin", "glslc.exe")
    return os.path.normpath(compiler_path)


def syntax() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Compiles GLSL shaders to SPIR-V.")
    parser.add_argument(
        "--shader_name", type=str, nargs="+",
        help="Shader name. All shader files which have the corresponding name will be compiled."
    )
    parser.add_argument("--shader_path", type=str, nargs="+", help="Shader path.")

    return parser


def main() -> None:
    args: argparse.Namespace = syntax().parse_args()

    # Shader Paths
    shader_paths: list[str] = [os.path.abspath(x) for x in args.shader_path or []]

    # Shader Names
    shader_names: set[str] = set(args.shader_name or [])
    if shader_names:
        for file in os.listdir("shaders"):
            if os.path.splitext(file)[0] in shader_names:
                shader_path: str = os.path.join("shaders", file)
                shader_paths.append(os.path.abspath(shader_path))
    
    spirv_compiler: str = find_spirv_compiler()
    for shader_path in shader_paths:
        shader_dir: str = os.path.dirname(shader_path)
        shader_name, shader_ext = os.path.splitext(os.path.basename(shader_path))

        spirv_path: str = os.path.join(shader_dir, f"{shader_name}_{shader_ext[1:]}.spv")
        subprocess.Popen([spirv_compiler, shader_path, "-o", spirv_path])



if __name__ == "__main__":
    main()
