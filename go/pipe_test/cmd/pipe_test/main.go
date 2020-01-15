package main

import(
  "context"
  "fmt"
  "io"
  "io/ioutil"
  "log"
  "os"
  ipc "bitbucket.org/avd/go-ipc/fifo"
  "os/signal"
)

const (
  pipe_name = "test_vol/TEST_FIFO"
)

var (
  global_count int64 = 0
)

func FifoRoutine(fifo *ipc.Fifo, ctx context.Context) {
  log.Println("Entering FIFO routine...")
  buf := make([]byte, 64)
  for {
    select {
    case <- ctx.Done():
      return
    default:
      num, err := (*fifo).Read(buf)
      // Switch on err, we don't care about EOF.
      switch err {
        case nil:
        case io.EOF:
          continue
        default:
          log.Println(err)
      }
      global_count += int64(num)
      log.Printf("Global count: %d\n", global_count)
    }
  }
}

func ls (dir string) {
  fmt.Fprintln(os.Stderr, "Printing directories of: " + dir)

  files, err := ioutil.ReadDir(dir)
  if err != nil {
    log.Fatal(err)
  }

  if len(files) == 0 {
    fmt.Fprintln(os.Stderr, "Empty")
    return
  }

  for _, f := range files {
    fmt.Fprintln(os.Stderr, f.Name())
  }
}

func main() {
  // Set up shutdown handlers.
  termChan := make(chan os.Signal, 1)
  signal.Notify(termChan, os.Interrupt, os.Kill)
  ctx, cancelFunc := context.WithCancel(context.Background())

  // Make the named pipe.
  fifo, err := ipc.New(pipe_name, os.O_RDONLY | os.O_CREATE, 0x666)
  if err != nil {
    log.Fatalf("Failed to open named FIFO: %s", err)
  }
  defer fifo.Close()

  go FifoRoutine(&fifo, ctx)

  // Block until shutdown.
  <-termChan
  log.Println("\nReceived an interrupt, stopping services...")
  cancelFunc()
}
