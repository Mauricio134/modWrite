cmd_/home/mauricioapaza/modWrite/chardev.mod := printf '%s\n'   chardev.o | awk '!x[$$0]++ { print("/home/mauricioapaza/modWrite/"$$0) }' > /home/mauricioapaza/modWrite/chardev.mod
