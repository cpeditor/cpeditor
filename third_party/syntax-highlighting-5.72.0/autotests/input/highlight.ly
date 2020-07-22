% This is a file to test the Lilypond highlighting features of Katepart.
% This is NOT a valid lilypond file, because it also shows the
% highlighting of some invalid lilypond constructs!
% This is a line comment.

%{
this is a block comment, that can occur inside a line, or across
multiple lines.
%}

\header {
  title = "Katepart Lilypond syntax highlighting test file"
  composer = %{"Wilbert Berendsen"%} "Anonymus"
  poet = "The KDE team"
  opus = "1"
  copyright = "Share and enjoy!"
}

\paper {
  #(set-paper-size "a4")  % a hash introduces scheme
  indent = 0              % recognize variable names inside \paper, \layout etc.
  between-system-space = 3\mm
}

\layout {
  \context {
    \Score
    \remove Bar_number_engraver   % recognize engraver names
    \remove "Bar_number_engraver" % also when quoted!
  }
}

% { and << block are folded
\score {
  \new StaffGroup <<
    \new ChordNames \chordmode { c2sus4/f g2/d c1 }
    \new Staff \new Voice ="mel" {
      \key f \major
      \time 4/4
      \partial 4
      \set Staff.instrumentName = "Bla."
      \once \override Staff.NoteHead #'color = #red
      % dynamics, articulations and markup
      c8\p d\< e-. f-> g a\f c-5\markup {Hoi}
      \notemode { c d e f }
      \repeat volta 2 {
        % complex durations are highlighted:
        g4*2/3
      }
      \times 2/3 {e8 f g}
      % there is some error checking:
      % often made mistake to have octavemarks after the duration:
      c2'
      % invalid durations are caught:
      g3
    }
    \context Lyrics \lyricsto "mel" {
      \set fontSize = #1
      this is a line of ly -- rics.
      with4 dur -- a -- tions.2.
      % errors like forgetting spaces are found:
      space-- flight %{i.s.o.%} space -- flight
      space at end for -- got -- ten as well.}
    
    \new DrumStaff \drummode { hihat4 bassdrum8 }
    \new FiguredBass \figuremode {
      <5 4>8 <6->
    }
  >>
  \midi {
    \context {
      \Score
      tempoWholesPerMinute = #(ly:make-moment 60 2)
    }
  }
}

av = #(define-music-function (parser location voice)
(string?)
; scheme comments are recognized in scheme
; Lilypond inside scheme works as well:
#{
  \set associatedVoice = $voice
#})



      
% inside scheme some elements are highlighted:    
#(define  (naturalise-pitch p)
  (let* ((o (ly:pitch-octave p))
         (a (* 4 (ly:pitch-alteration p))) 
         (n (ly:pitch-notename p)))
         (bla 'ArticulationEvent 'ChoirStaff)
    (cond
     ((> a 2) (set! a (- a 4)) (set! n (+ n 1)))
     ((< a -2) (set! a (+ a 4)) (set! n (- n 1))))

    (if (< n 0) (begin (set!  o (- o 1)) (set! n (+ n 7))))
    (if (> n 6) (begin (set!  o (+ o 1)) (set! n (- n 7))))

    (ly:make-pitch o n (/ a 4))))

% markup is also highlighted
\markup {
  \line {
    text test Voice Staff % note Lilypond keywords are not highlighted here
  }
  \score { \relative c' { <ceg>2( d) } }
  \italic bla
  \override #'(baseline-skip . 2) {
    \underline blu
  }
}
