import argparse
import os
import subprocess


def find_compiler_path() -> str | None:
	vulkan_directory: str = os.environ.get("VK_SDK_PATH")
	if not vulkan_directory:
		vulkan_directory = os.environ.get("VULKAN_SDK")
	if not vulkan_directory:
		return None
	
	compiler_path: str = os.path.join(vulkan_directory, "Bin", "glslc.exe")
	return compiler_path if os.path.exists(compiler_path) else None


def parser_syntax() -> argparse.ArgumentParser:
	parser = argparse.ArgumentParser(description="Compiles a shader to generate SPV files")
	parser.add_argument("--shader_name", type=str, nargs="+", help="Shader name - Compiles all the shaders with this name.")
	parser.add_argument("--shader_path", type=str, nargs="+", help="Path of the specific shaders to compile")

	return parser


def compile_by_path(compiler_path: str, shader_path: str) -> None:
	shader_directory: str = os.path.dirname(shader_path)
	name, extension = os.path.splitext(os.path.basename(shader_path))

	spv_path: str = os.path.join(shader_directory, f"{name}_{extension[1:]}.spv")
	subprocess.run([compiler_path, shader_path, "-o", spv_path])


def compile_by_name(compiler_path: str, shader_name: str) -> None:
	root_directory: str = os.path.dirname(__file__)
	for file in os.listdir(root_directory):
		file_name: str = file.split(".")[0]
		if file_name == shader_name:
			compile_by_path(compiler_path, os.path.join(root_directory, file))


def main() -> None:
	args: argparse.Namespace = parser_syntax().parse_args()

	compiler_path: str = find_compiler_path()
	if not compiler_path:
		raise RuntimeError("Cannot find the compiler path")
	
	for shader_name in (args.shader_name or []):
		compile_by_name(compiler_path, shader_name)
	for shader_path in (args.shader_path or []):
		compile_by_path(compiler_path, shader_path)


if __name__ == "__main__":
	main()
