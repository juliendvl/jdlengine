import argparse
import logging
import os
import shutil
import subprocess


logging.basicConfig(level=logging.INFO)


def get_slang_compiler() -> str:
    if os.name == "nt":
        vulkan_directory: str = os.environ.get("VULKAN_SDK")
        if not vulkan_directory:
            vulkan_directory = os.environ.get("VK_SDK_PATH")
        if not vulkan_directory:
            raise RuntimeError("Cannot find the Vulkan SDK directory")

        slang_compiler: str = os.path.join(vulkan_directory, "Bin", "slangc.exe")
        if not os.path.exists(slang_compiler):
            raise RuntimeError("Cannot find slangc.exe in the Vulkan SDK directory")

        return slang_compiler
    elif os.name == "posix":
        slang_compiler: str = shutil.which("slangc")
        if slang_compiler is None:
            raise RuntimeError("Cannot find the 'slangc' command")
        return slang_compiler
    else:
        raise NotImplementedError(f"Unsupported platform: {os.name}")


def syntax() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Compiles Slang shaders to SPIR-V")
    parser.add_argument(
        "--shader_name", type=str, nargs="+",
        help="Shader name (internal shaders located in the shaders directory)"
    )
    parser.add_argument(
        "--shader_path", type=str, nargs="+",
        help="Shader path (external shaders)"
    )

    return parser


def main() -> None:
    args: argparse.Namespace = syntax().parse_args()

    slang_compiler: str = get_slang_compiler()
    logging.info(f"Using Slang compiler: {slang_compiler}")

    shader_paths: list[str] = [os.path.normpath(f) for f in args.shader_path or []]
    if args.shader_name:
        script_directory: str = os.path.dirname(__file__)
        shader_paths.extend(
            os.path.join(script_directory, f"{s}.slang") for s in args.shader_name
        )

    for shader_path in shader_paths:
        path_split: list[str] = shader_path.split(".")
        logging.info(f"Compiling {shader_path} ...")

        compiler_args: list[str] = [
            slang_compiler,
            shader_path,
            "-target", "spirv",
            "-profile", "spirv_1_4",
            "-emit-spirv-directly",
            "-fvk-use-entrypoint-name",
            "-entry", "vert_main",
            "-entry", "frag_main",
            "-o", f"{path_split[0]}.spv",
        ]
        subprocess.Popen(compiler_args)


if __name__ == "__main__":
    main()
