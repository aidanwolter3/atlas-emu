def _rom_impl(ctx):
    assembly_files = ctx.files.asms
    rom_file_path = ctx.label.name + ".nes"
    rom_file = ctx.actions.declare_file(rom_file_path)
  
    # Assemble the input assembly files into object files.
    object_files = []
    for asm in assembly_files:
        object_file_path = asm.path[:-len(asm.extension)] + "o"
        object_file = ctx.actions.declare_file(object_file_path)
        object_files.append(object_file)
        ctx.actions.run(
            inputs = assembly_files,
            outputs = [object_file],
            executable = ctx.executable._ca65,
            arguments = ["-o", object_file.path, asm.path],
        )
  
    # Create the arguments to pass to the linker.
    linker_args = ctx.actions.args()
    linker_args.add("--target", "nes")
    # TODO: Generate this path instead of hardcoding
    linker_args.add("--cfg-path", "bazel-out/host/bin/external/cc65/ld65.runfiles/cc65/cfg")
    linker_args.add("-o", rom_file.path)
    for o in object_files:
        linker_args.add(o.path)
  
    # Link the object files and the input libraries into a ROM file.
    ctx.actions.run(
        inputs = object_files,
        outputs = [rom_file],
        executable = ctx.executable._ld65,
        arguments = [linker_args],
    )
  
    # Return the ROM file.
    return [DefaultInfo(runfiles = ctx.runfiles([rom_file]))]

rom = rule(
    implementation = _rom_impl,
    attrs = {
        "_ca65": attr.label(
            default = "@cc65//:ca65",
            executable = True,
            cfg = "host",
        ),
        "_ld65": attr.label(
            default = "@cc65//:ld65",
            executable = True,
            cfg = "host",
        ),
        "asms": attr.label_list(
            allow_files = [".s"],
        ),
    },
)
