import java.util.Scanner;

//--PASSO 1: cria uma classe que estende a classe Thread 
class Pi_conc extends Thread {
    private int id;
    private int numero_de_threads;
    private int tamanho_do_loop;
    private double pi_conc_thread;
    
    //--construtor
    public Pi_conc(int thread_id, int N, int n, double pi_conc) { 
        this.id = thread_id;
        this.numero_de_threads = N;
        this.tamanho_do_loop = n;
        this.pi_conc_thread = pi_conc;
    }

    //--metodo executado pela thread ( nosso algoritmo do Pi )
    public void run() {
        for (int i = id; i < tamanho_do_loop; i+=numero_de_threads) {
            pi_conc_thread += Math.pow(-1, i) / (2 * i + 1);
        }
        main.pi_conc += pi_conc_thread;
    }
}

//--classe do metodo main
class main {
   public static double pi_seq = 0;
   public static double pi_conc = 0;

   public static void main (String[] args) {

        Scanner scanner = new Scanner(System.in);
        System.out.print("Digite qual será nosso \"n\" para a fórmula do PI: ");
        int n = scanner.nextInt();

        System.out.print("Quantas threads iremos usar? ");
        int N = scanner.nextInt();

        //--reserva espaço para um vetor de threads
        Thread[] threads = new Thread[N];
      
        //-- vou fazer um for para resolver o lab 3 primeiramente de forma sequencial
        for (int i = 0; i < n; i++) {
            pi_seq += Math.pow(-1, i) / (2 * i + 1);
        }

        //--PASSO 2: cria threads da classe que estende Thread
        int thread_id = 0;
        for (int i=0; i<threads.length; i++) {
            thread_id = i;
            threads[i] = new Pi_conc(thread_id, N, n, 0);
        }

        //--PASSO 3: iniciar as threads
        for (int i=0; i<threads.length; i++) {
            threads[i].start();
        }

        //--PASSO 4: esperar pelo termino de todas as threads
        for (int i=0; i<threads.length; i++) {
            try { threads[i].join(); } 
            catch (InterruptedException e) { return; }
        }

        //-- Peguei um valor de Pi na internet e calculei também de forma sequencial, para verificar
        //-- se o erro é maior que 8 casas decimais. E não foi o caso pelo menos na minha máquina.
        //-- para melhor desempenho do código, comentar a parte do erro e lá encima o calculo sequencial.
        double erro1 = Math.abs(3.14159265358979323846 - 4*pi_conc);
        double erro2 = Math.abs(4*pi_seq - 4*pi_conc);

        System.out.println("PI concorrente: " + pi_conc * 4);
        System.out.println("PI sequencial: " + pi_seq * 4);
        //-- Tento ver se o erro é maior que de 8 casas decimais.
        System.out.printf("Erro do pi real - pi concorrente: %.9f\n" , erro1 );
        System.out.printf("Erro do pi sequencial - pi concorrente: %.9f\n" , erro2 );
    
        System.out.println("Terminou"); 
   }
}
