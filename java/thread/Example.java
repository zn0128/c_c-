public class Example {
    private volatile boolean flag = true;

    private class MyThread extends Thread {
        @Override
        public void run() {
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            // 修改变量值
            flag = false;
            for (; ; ) {
                if (flag) {
                    System.out.println("flag = " + flag);
                } else {
                    System.out.println("flag = " + flag);
                }
            }
        }
    }

    private class MyThread2 extends Thread {
        @Override
        public void run() {
            for (; ; ) {
                if (flag) {
                    flag = false;
                } else {
                    flag = true;
                }
            }
        }
    }

    public static void main(String[] args) {
        Example test = new Example();

        MyThread myThread1 = test.new MyThread();
        MyThread2 myThread2 = test.new MyThread2();
        // 开启线程
        myThread1.start();
        try {
            Thread.sleep(5000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        myThread2.start();
	}
}
