package main

import (
	"context"
	"flag"
	"fmt"
	"os"
	//"strings"

	"github.com/google/subcommands"
)

type printCmd struct {
	r bool
	v bool
}

func (*printCmd) Name() string     { return "print" }
func (*printCmd) Synopsis() string { return "Print args to stdout." }
func (*printCmd) Usage() string {
	return `print [-r] [-v] <some text>:
  Print args to stdout.
`
}

func (p *printCmd) SetFlags(f *flag.FlagSet) {
	f.BoolVar(&p.r, "r", false, "r output")
	f.BoolVar(&p.v, "v", false, "v output")
}

func (p *printCmd) Execute(_ context.Context, f *flag.FlagSet, _ ...interface{}) subcommands.ExitStatus {
	if p.r {
		//arg = strings.ToUpper(arg)
		fmt.Println("set r")
	}

	if p.v {
		fmt.Println("set v")
	}

	fmt.Println(f.name)

	for _, arg := range f.Args() {
		fmt.Printf("%s\n", arg)
	}

	f.Visit(func(f *flag.Flag) {
		fmt.Println(f.Name)
	})

	fmt.Println()
	return subcommands.ExitSuccess
}

func main() {
	subcommands.Register(subcommands.HelpCommand(), "")
	subcommands.Register(subcommands.FlagsCommand(), "")
	subcommands.Register(subcommands.CommandsCommand(), "")
	subcommands.Register(&printCmd{}, "")

	flag.Parse()
	ctx := context.Background()
	os.Exit(int(subcommands.Execute(ctx)))
}
