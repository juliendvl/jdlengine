import argparse
import logging
import os
import shutil
import subprocess
import sys


logging.basicConfig(
    level=logging.INFO,
    format="[%(levelname)s][%(filename)s] %(message)s"
)


def find_slang_compiler() -> str:
    if sys.platform == "win32":
        sdk_path: str = os.environ.get("VULKAN_SDK") or ""
        return os.path.join(sdk_path, "Bin", "slangc.exe")
    elif sys.platform.startswith("linux"):
        return shutil.which("slangc") or ""
    else:
        raise RuntimeError(f"Unsupported platform: {sys.platform}")


def syntax() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Compiles SLANG shaders to SPIR-V.")
    parser.add_argument(
        "--name", type=str, nargs="+", help="Shader name (for built-in shaders)."
    )
    parser.add_argument(
        "--path", type=str, nargs="+", help="Shader path (for external shaders)."
    )
    return parser


def main() -> None:
    args: argparse.Namespace = syntax().parse_args()

    shader_paths: list[str] = [os.path.abspath(p) for p in args.path or []]
    if args.name:
        root_path: str = os.path.abspath(os.path.dirname(__file__))
        shader_paths.extend(os.path.join(root_path, f"{n}.slang") for n in args.name)

    if not shader_paths:
        logging.error("No shaders to compile.")
        return

    slang_compiler: str = find_slang_compiler()
    if not os.path.exists(slang_compiler):
        logging.error("Cannot find the SLANG compiler.")
        return
    else:
        logging.info(f"Using SLANG compiler: {slang_compiler}")

    for shader_path in shader_paths:
        basename, _ = os.path.splitext(shader_path)
        output_path: str = basename + ".spv"

        logging.info(f"Compiling shader {shader_path}")
        process = subprocess.Popen(
            [
                slang_compiler,
                shader_path,
                "-target",
                "spirv",
                "-profile",
                "spirv_1_4",
                "-emit-spirv-directly",
                "-fvk-use-entrypoint-name",
                "-entry",
                "vert_main",
                "-entry",
                "frag_main",
                "-o",
                output_path,
            ]
        )
        process.wait()


if __name__ == "__main__":
    main()
