#!/bin/bash

env -i valgrind --leak-check=full --track-origins=yes --suppressions=./minishell.supp ./minishell
