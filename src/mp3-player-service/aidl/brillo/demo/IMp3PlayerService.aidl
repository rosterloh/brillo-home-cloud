package brillo.demo;

interface IMp3PlayerService {
	void play();
	void pause();
	void stop();
	boolean reachedEOS();
	String status();
}
